TARGET = obj/ddcutil_gui

# Pastas
SRCDIR = src
OBJDIR = obj

# Arquivos de origem
BLUEPRINT     = $(SRCDIR)/window.blp
RESOURCES_XML = $(SRCDIR)/resources.gresource.xml

# Arquivos gerados
INTERFACE     = $(OBJDIR)/window.ui
RESOURCES_C   = $(OBJDIR)/resources.c

# Fontes (monitor agora é .cpp)
SRCS_CPP = $(SRCDIR)/main.cpp $(SRCDIR)/MainWindow.cpp $(SRCDIR)/Monitor.cpp
# Objetos (todos gerados a partir de .cpp ou do recurso .c)
OBJS = $(OBJDIR)/main.o $(OBJDIR)/MainWindow.o $(OBJDIR)/Monitor.o $(OBJDIR)/resources.o

# Ferramentas
CC = gcc
CXX = g++
BLUEPRINT_COMPILER = blueprint-compiler
RESOURCE_COMPILER = glib-compile-resources

# Flags
GTKMM_FLAGS = `pkg-config --cflags --libs gtkmm-4.0`
GTK4_FLAGS = `pkg-config --cflags --libs gtk4`
CXXFLAGS_BASE = -Wall -std=c++17 -I$(SRCDIR)
CFLAGS_BASE = -Wall -I$(SRCDIR)

# --- REGRAS ---

all: debug

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Linkagem Final (Usa CXX para linkar objetos C++ e C)
$(TARGET): $(OBJDIR) $(INTERFACE) $(RESOURCES_C) $(OBJS)
	$(CXX) $(OBJS) $(GTKMM_FLAGS) -o $(TARGET)

# Compilação C++ (main, MainWindow e Monitor)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags gtkmm-4.0` -c $< -o $@

# Compilação do recurso gerado (O recurso gerado é código C puro)
$(OBJDIR)/resources.o: $(RESOURCES_C)
	$(CC) $(CFLAGS) `pkg-config --cflags gtk4` -c $< -o $@

$(INTERFACE): $(BLUEPRINT) | $(OBJDIR)
	$(BLUEPRINT_COMPILER) compile --output $(INTERFACE) $(BLUEPRINT)

$(RESOURCES_C): $(RESOURCES_XML) $(INTERFACE) | $(OBJDIR)
	$(RESOURCE_COMPILER) $(RESOURCES_XML) --target=$(RESOURCES_C) --generate-source --sourcedir=$(SRCDIR) --sourcedir=$(OBJDIR)

# Modos de Build
debug: CXXFLAGS = $(CXXFLAGS_BASE) -g
debug: CFLAGS = $(CFLAGS_BASE) -g
debug: $(TARGET)

release: CXXFLAGS = $(CXXFLAGS_BASE) -O3
release: CFLAGS = $(CFLAGS_BASE) -O3
release: $(TARGET)
	strip $(TARGET)

# Geração do compile_commands.json
compiledb: clean
	@mkdir -p $(OBJDIR)
	bear -- make debug
	@mv compile_commands.json $(OBJDIR)/ 2>/dev/null || true
	@echo "📂 compile_commands.json atualizado em $(OBJDIR)/"

clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "🧹 Limpeza concluída."

run: debug
	./$(TARGET)

.PHONY: all debug release clean compiledb run