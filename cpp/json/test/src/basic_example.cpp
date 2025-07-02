#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json; 

int main() {
    // Create a JSON object
    json j;
    j["name"] = "John Doe";
    j["age"] = 30;
    j["city"] = "New York";

    // Serialize the JSON object to a string
    std::string serialized = j.dump(4); // 4 spaces for indentation
    std::cout << "Serialized JSON:\n" << serialized << std::endl;

    // Parse the JSON string back to a JSON object
    json parsed = json::parse(serialized);
    std::cout << "\nParsed JSON:\n";
    std::cout << "Name: " << parsed["name"].get<std::string>() << std::endl;
    std::cout << "Age: " << parsed["age"].get<int>() << std::endl;
    std::cout << "City: " << parsed["city"].get<std::string>() << std::endl;

    // Additional Use Cases for nlohmann/json Library
    std::cout << "\n=== Additional Use Cases for nlohmann/json ===\n";

    // Use Case 1: Working with Arrays
    std::cout << "\nUse Case 1: Working with Arrays\n";
    json arrayExample;
    arrayExample["numbers"] = {1, 2, 3, 4, 5};
    arrayExample["fruits"] = json::array({"apple", "banana", "orange"});
    std::cout << "Array Example:\n" << arrayExample.dump(4) << std::endl;

    // Use Case 2: Nested Objects
    std::cout << "\nUse Case 2: Nested Objects\n";
    json nestedExample;
    nestedExample["person"]["name"] = "Jane Doe";
    nestedExample["person"]["address"]["street"] = "123 Main St";
    nestedExample["person"]["address"]["city"] = "Los Angeles";
    nestedExample["person"]["address"]["zip"] = 90001;
    std::cout << "Nested Object Example:\n" << nestedExample.dump(4) << std::endl;

    // Use Case 3: Different Data Types
    std::cout << "\nUse Case 3: Different Data Types\n";
    json typesExample;
    typesExample["integer"] = 42;
    typesExample["floating"] = 3.14;
    typesExample["boolean"] = true;
    typesExample["null"] = nullptr;
    typesExample["string"] = "Hello, JSON!";
    std::cout << "Data Types Example:\n" << typesExample.dump(4) << std::endl;

    // Use Case 4: Error Handling during Parsing
    std::cout << "\nUse Case 4: Error Handling during Parsing\n";
    std::string invalidJson = "{ \"key\": \"value\" "; // Missing closing brace
    try {
        json errorExample = json::parse(invalidJson);
    } catch (const json::parse_error& e) {
        std::cout << "Parse Error: " << e.what() << std::endl;
    }

    // Use Case 5: Checking for Key Existence and Type Safety
    std::cout << "\nUse Case 5: Checking for Key Existence and Type Safety\n";
    json safetyExample;
    safetyExample["data"] = 100;
    if (safetyExample.contains("data") && safetyExample["data"].is_number()) {
        int value = safetyExample["data"].get<int>();
        std::cout << "Safely accessed 'data': " << value << std::endl;
    } else {
        std::cout << "Key 'data' not found or not a number." << std::endl;
    }

    // Use Case 6: Printing Inline JSON
    std::cout << "\nUse Case 6: Printing Inline JSON\n";
    json inlineExample;
    inlineExample["name"] = "Inline JSON";
    inlineExample["value"] = 12345;
    std::cout << "Inline JSON Example: " << inlineExample.dump() << std::endl;

    // Use Case 7: Parsing Inline JSON
    std::cout << "\nUse Case 7: Parsing Inline JSON\n";
    std::string inlineJsonStr = "{\"example\": \"parsed\", \"number\": 42}";
    json parsedInline = json::parse(inlineJsonStr);
    std::cout << "Parsed Inline JSON:\n";
    std::cout << "Example: " << parsedInline["example"].get<std::string>() << std::endl;
    std::cout << "Number: " << parsedInline["number"].get<int>() << std::endl;

    return 0;
}
