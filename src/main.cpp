#define CPPHTTPLIB_OPENSSL_SUPPORT 0
#include "ExternalDependencies/ollama_fixed.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <vector>

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
    
    // Display models with numbers
    std::vector<std::string> model_names;
    for (size_t i = 0; i < models.size(); i++) {
        std::cout << "  " << (i + 1) << ". " << models[i] << std::endl;
        model_names.push_back(models[i]);
    }
    
    // Select model
    std::string user_input;
    std::string model_name;
    bool valid_selection = false;
    
    while (!valid_selection) {
        std::cout << "\nSelect model by number (1-" << models.size() << ") or enter model name: ";
        std::getline(std::cin, user_input);
        
        // Check if input is a number
        try {
            int selection = std::stoi(user_input);
            if (selection >= 1 && selection <= static_cast<int>(models.size())) {
                model_name = model_names[selection - 1];
                valid_selection = true;
            } else {
                std::cout << "Invalid selection. Please try again." << std::endl;
            }
        } catch (const std::exception&) {
            // Input is not a number, treat as model name
            model_name = user_input;
            valid_selection = true;
        }
    }
    
    // Initialize chat session
    ollama::messages chat_history;
    
    // Add system message if desired
    chat_history.add_system("You are a helpful AI assistant.");
    
    std::string user_message;
    
    std::cout << "\nChat started with " << model_name << ". Type 'exit' to quit.\n" << std::endl;
    
    while (true) {
        // Get user input
        std::cout << "You: ";
        std::getline(std::cin, user_message);
        
        // Check for exit command
        if (user_message == "exit" || user_message == "quit") {
            break;
        }
        
        // Add user message to history
        chat_history.add_user(user_message);
        
        try {
            // Generate response
            std::cout << "Waiting for response..." << std::endl;
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