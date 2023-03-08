#include <iostream>
#include <thread>
#include <list>
#include <utility> // Includes std::pair


struct Queue {
    public:
        // The queue itself - a list of pairs
        std::list<std::pair<std::string, int>> queue;

        // Adds a pair to the end of the queue
        void add (std::pair<std::string, int> to_add) {
            queue.push_back (to_add);
        };

        // Removes the pair from the front of the queue
        std::pair<std::string, int> remove (){

            // Stores the first element of the queue
            std::pair<std::string, int> removed = queue.front();

            // Removes the first element
            queue.pop_front();

            // Returns the removed element
            return removed;
        };

        // Populates the queue with 55 string, int pairs
        void populate (){
            int no_items = 55; // Number of items placed in the queue

            std::string alphabet = "abcdefghijklmnopqrstuvwxyz"; // Reference alphabet for generating strings

            // Loop through and create no_items (string, int) pairs
            for (int i=0; i < no_items; i++){
                int str_len = rand() % 4 + 2; // Generate a number between 2 and 5 inclusive

                std::string temp_str = ""; // Initialise empty string
                // Loop through and create a string of length str_len
                for (int j=0; j < str_len; j++){
                    temp_str += alphabet[rand() % 26]; // Add random letter to string
                };

                int temp_int = rand() % 256; // Randomly generate a number between 0 and 255

                add(std::make_pair(temp_str, temp_int)); // Add the pair to the list

                std::cout<<"temp_str: "<<temp_str<<", temp_int: "<<temp_int<<std::endl; //Just for checking

            };
        };
};

void test(Queue q) {

    /*
    Queue q;
    q.add(std::make_pair(std::string("abc"), 6));
    q.add(std::make_pair(std::string("def"), 7));
    std::pair<std::string, int> removed = q.remove();
    
    std::string a = q.queue.front().first;
    int b = q.queue.front().second;

    std::cout<<"Queue now contains: "<<a<<", "<<b<<std::endl;
    std::cout<<"After removing: "<<removed.first<<", "<<removed.second<<std::endl;
    */

   Queue p;
   p.populate();

   std::cout<<p.queue.size()<<std::endl;
}

void main() {

    // Create the queue
    Queue q;
    // Populate the queue
    q.populate();

}