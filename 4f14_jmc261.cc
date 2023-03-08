#include <iostream>
#include <thread>
#include <vector>
#include <utility> // Includes std::pair
#include <mutex>
#include <tuple>
#include <chrono>

struct Queue {
    public:
        // The queue itself - a list of pairs
        std::vector<std::pair<std::string, int>> queue;

        // Set up mutex for processing
        std::mutex m;
        
        // Adds a pair to the end of the queue
        void add (std::pair<std::string, int> to_add) {
            queue.push_back (to_add);
        };

        // Removes the pair from the front of the queue
        std::pair<std::string, int> remove (){

            // Stores the first element of the queue
            std::pair<std::string, int> removed = queue.front();

            // Removes the first element
            queue.erase(queue.begin());

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

                //std::cout<<"temp_str: "<<temp_str<<", temp_int: "<<temp_int<<std::endl; //Just for checking

            };
        };
        
        // Function used in 'reverse' to swap two numbers
        void lock_and_swap (int index) {
            // Lock the queue while swap occurs
            std::lock_guard<std::mutex> guard(m);

            int max_index = queue.size()/2; // int acts as a floor function

            // Need to recheck since list could have been altered since while in 'reverse'
            if (index < max_index) {
                // Swap
                std::iter_swap(queue.begin() + index, queue.begin() + (queue.size()-1) - index);
            }
            else if (queue.size() == 0) {
                ; // Pass
            }
            else {
                // Sum if index >= max_index
                int sum = 0;
                for (std::pair<std::string, int> item : queue) {
                    sum += item.second;
                };
                // Print the sum
                std::cout<<"(c) - Sum of reversed integers: "<<sum<<std::endl;
            }
        };

        // Reverse the list
        void reverse_c () {
            while (queue.size() > 0) {
            //for (int j = 0; j<4; j++){ for testing
                int i = 0;
                int len_q = queue.size()/2;
                while (i < len_q + 1) {
                    lock_and_swap(i);
                    i++;
                };
            };
        };


        // Updates k if needed and returns the new pair
        std::tuple<int, std::pair<std::string, int>> get_item (int k) {
            // Lock the vector
            std::lock_guard<std::mutex> guard(m);

            // If deletion occurs between while loop and get_item call, exit function
            if (queue.size() == 0) {
                return std::make_tuple(-1, std::make_pair(" ", 0));
            }
            // If k is too big for vector, reset it
            else if (k >= queue.size()) {
                k = 0;
            }
            return std::make_tuple(k, queue[k]);

        }

        // Continually prints the string and integer values currently present in the queue
        // Only use the mutex for obtaining the values, don't need it for printing (slow)
        void cont_print_d () {
            int k = 0; // Index of element currently printing
            std::pair<std::string, int> item_k;

            while (queue.size() > 0) {
            //for (int z = 0; z < 20; z++) { for testing
                tie(k, item_k) = get_item(k);
                if (k == -1) {
                    // Break the loop if queue size drops to 0
                    break;
                }
                else {
                    std::cout<<"(d) - Item "<<k<<" contains: "<<item_k.first<<", "<<item_k.second<<std::endl;
                    k++;
                }
            };
        };


        // Locks the queue and deletes the item at index 'to_delete'
        void delete_item (int to_delete) {
            // Lock the vector
            std::lock_guard<std::mutex> guard(m);

            // Erase the item at index 'to_delete'
            queue.erase(queue.begin() + to_delete);
        }

        // Randomly delete one item every 0.3s
        void delete_random_e () {
            while (queue.size() > 0) {
                // Sleep for 0.3s
                std::this_thread::sleep_for(std::chrono::seconds(3 / 10));
                
                /*
                //For testing
                std::this_thread::sleep_for(std::chrono::seconds(6));
                std::cout<<"Deleting"<<std::endl;
                // This snippet is HIGHLY slowed down by the time taken for cout
                */

                // Randomly choose which item to delete
                int to_delete = rand() % queue.size();

                // Delete the item
                delete_item(to_delete);
            }
        }

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

int main() {

    // Create the queue
    Queue q;

    // Populate the queue
    q.populate();

    // Start the first background thread, part (c)
    std::thread t1(&Queue::reverse_c, std::ref(q));

    // Start the second background thread, part (d)
    std::thread t2(&Queue::cont_print_d, std::ref(q));

    // Start the final background thread, part (e)
    std::thread t3(&Queue::delete_random_e, std::ref(q));

    // t3 will ALWAYS finish first (as (c) and (d) can't finish until (e) is)
    t3.join();
    

    // t1 and t2 will finish randomly between them, just ensure they finish before the
    // program ends
    t1.join();
    t2.join();

    std::cout<<"Finished successfully"<<std::endl;

    return 0; // Completed successfully

}