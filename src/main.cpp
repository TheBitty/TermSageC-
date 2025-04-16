#include "ExternalDependencies/ollama.hpp" // for our AI model

int main() {
    ollama::list_running_models();
    //start the ollama model
    //list available models( all of which should be in a settings separate from the main terminal )
    return 0;
}