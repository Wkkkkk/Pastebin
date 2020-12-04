// MongoTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

int main()
{
    std::cout << "Hello World!\n";

    mongocxx::instance inst{}; // This should be done only once.
    mongocxx::client conn{ 
        mongocxx::uri{
            "mongodb+srv://Wkkkkk:wkkkkk@cluster0.wqx0r.mongodb.net/Pastebin?retryWrites=true&w=majority"
        } 
    };
    mongocxx::database database = conn["Pastebin"];
    mongocxx::collection collection = database["Test"];

    {
        bsoncxx::builder::stream::document document{};
        document << "hello" << "world2";

        collection.insert_one(document.view());
    } 

    {
        bsoncxx::builder::stream::document document{};
        document << "hello" << "world3";

        collection.insert_one(document.view());
    }

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        collection.find_one(bsoncxx::builder::stream::document() << "hello" << "world" << bsoncxx::builder::stream::finalize);
    if (maybe_result) {
        std::cout << bsoncxx::to_json(*maybe_result) << "\n";
    }

    //auto cursor = collection.find({});
    //for (auto&& doc : cursor) {
    //    std::cout << bsoncxx::to_json(doc) << std::endl;
    //}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
