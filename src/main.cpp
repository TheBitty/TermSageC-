#include "ExternalDependencies/ollama_fixed.hpp"
#include <iostream>
#include <string>
#include <limits>

int main() {
    // Create an Ollama instance
    Ollama ollama;
    
    // Check if Ollama server is running
    if (!ollama.is_running()) {
        std::cout << "Ollama server is not running. Please start it first with 'ollama serve'." << std::endl;
        return 1;
    }
    
    std::cout << "Connected to Ollama server." << std::endl;
    
    // List available models
    std::cout << "\nAvailable models:" << std::endl;
    auto models = ollama.list_models();
    if (models.empty()) {
        std::cout << "  No models found. Please pull at least one model (e.g., 'ollama pull llama3')." << std::endl;
        return 1;
    }
    
    for (const auto& model : models) {
        std::cout << "  - " << model << std::endl;
    }
    
    // Select model
    std::string model_name;
    std::cout << "\nEnter model name to chat with: ";
    std::getline(std::cin, model_name);
    
    // Initialize chat session
    ollama::messages chat_history;
    
    // Add system message if desired
    chat_history.add_system("You are a helpful AI assistant.");
    
    std::string user_input;
    
    std::cout << "\nChat started with " << model_name << ". Type 'exit' to quit.\n" << std::endl;
    
    while (true) {
        // Get user input
        std::cout << "You: ";
        std::getline(std::cin, user_input);
        
        // Check for exit command
        if (user_input == "exit" || user_input == "quit") {
            break;
        }
        
        // Add user message to history
        chat_history.add_user(user_input);
        
        try {
            // Generate response
            auto response = ollama.chat(model_name, chat_history);
            
            // Check for errors
            if (response.has_error()) {
                std::cerr << "Error: " << response.get_error() << std::endl;
                continue;
            }
            
            // Get assistant's response
            std::string assistant_response = response.as_simple_string();
            std::cout << "\nAssistant: " << assistant_response << "\n" << std::endl;
            
            // Add assistant's response to history
            chat_history.add_assistant(assistant_response);
        } 
        catch (const ollama::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    
    std::cout << "Chat ended." << std::endl;
    return 0;
}