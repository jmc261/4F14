#include <iostream>
#include <thread>
#include <vector>
#include <utility> // Includes std::pair
#include <mutex>
#include <tuple>
#include <chrono>
#include <exception>

struct Queue {
    public:
        // The queue itself - a list of pairs
        std::vector<std::pair<std::string, int>> queue;

        // Set up mutex for processing
        std::mutex m_process;
        // Set up mutex for printing (otherwise prints will be jumbled together)
        std::mutex m_print;


        //// CORE FUNCTIONS FOR PARTS (a) AND (b)
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


        //// FUNCTIONS FOR PARTS (c), (d) and (e)
        // Function used in 'reverse' to swap two numbers
        std::pair<int, bool> lock_and_swap (int index) {
            // Lock the queue while swap occurs
            std::lock_guard<std::mutex> guard(m_process);

            int max_index = queue.size()/2; // int acts as a floor function

            // Need to recheck since list could have been altered since while in 'reverse'
            if (index < max_index) {
                // Swap
                std::iter_swap(queue.begin() + index, queue.begin() + (queue.size()-1) - index);
                return std::make_pair(index+1, true);
            }
            else if (queue.size() == 0) {
                return std::make_pair(-1, false); // Exit loop, -1 indicates this is NOT a sum (see reverse_c if statement)
            }
            else {
                // Sum if index >= max_index
                int sum = 0;
                for (std::pair<std::string, int> item : queue) {
                    sum += item.second;
                };
                // Use i (the first component of the return pair) as a surrogate to return the sum which can then
                // be printed
                return std::make_pair(sum, false); // Reset counter and restart reverse
            }
        };

        // Reverse the list
        void reverse_c () {
            while (queue.size() > 0) {
            //for (int j = 0; j<4; j++){ for testing
                // Define variables
                int i = 0;
                bool active = true;
                std::pair<int, bool> temp;

                while (active) {
                    temp = lock_and_swap(i); // Store the pair output of fn

                    // Unpack the pair
                    i = temp.first;
                    active = temp.second;
                };
                // 'sum' is stored in i. If i=-1 and 'active' is false, the thread should end
                // if i != -1, it must be the sum result, thus print and restart the reversal
                if (i != -1) {
                        // Print the sum
                        m_print.lock(); // Lock print mutex to prevent jumbling
                        std::cout<<"(c) - Sum of reversed integers: "<<i<<std::endl;
                        m_print.unlock();
                    }
                // i will be reset at the start of the next while iteration, so there shouldn't be
                // any indexing problems
            };
        };


        // Possibly faster version of get_item, also throws error
        std::pair<int, std::pair<std::string, int>> get_item (int k) {
            // Lock the vector
            std::lock_guard<std::mutex> guard(m_process);

            //// Even if queue.size() == 0, k cannot be less than queue.size()
            //// Therefore rearrange if statement to be most common

            // If deletion occurs between while loop and get_item call, exit function
            if (k < queue.size()) {
                return std::make_pair(k, queue[k]);
            }
            // Instructs thread to end
            else if (queue.size() == 0) {
                throw std::out_of_range( "Queue is empty" );
            }
            else { // i.e k >= queue.size() != 0
                return std::make_pair(0, queue[0]);
            }


        }

        // Continually prints the string and integer values currently present in the queue
        // Only use the mutex m_process for obtaining the values, don't need it for printing (slow)
        // Try-catch loop avoids the need for if-else
        void cont_print_d () {
            int k = 0; // Index of element currently printing
            std::pair<std::string, int> item_k;
            std::pair<int, std::pair<std::string, int>> temp2;

            while (queue.size() > 0) {
            //for (int z = 0; z < 20; z++) { for testing

                // Try-catch avoids if else statement
                try {
                    // Get the item at k (assuming the list hasn't changed length)
                    temp2 = get_item(k);
                    k = temp2.first;
                    item_k = temp2.second;
                    
                    // Print the item
                    m_print.lock(); // Lock print mutex to prevent jumbling
                    std::cout<<"(d) - Item "<<k<<" contains: "<<item_k.first<<", "<<item_k.second<<std::endl;
                    m_print.unlock();
                    k++;
                }
                catch (...) {
                    // get_item will throw an error if the queue is empty, so just catch it and break loop
                    break;
                }
            };
        };


        // Locks the queue and deletes the item at index 'to_delete'
        void delete_item (int to_delete) {
            // Lock the vector
            std::lock_guard<std::mutex> guard(m_process);

            // Erase the item at index 'to_delete'
            queue.erase(queue.begin() + to_delete);
        }

        // Randomly delete one item every 0.3s
        void delete_random_e () {
            while (queue.size() > 0) {
                // Sleep for 0.3s
                std::this_thread::sleep_for(std::chrono::milliseconds(300));

                // Randomly choose which item to delete
                int to_delete = rand() % queue.size();

                // Delete the item
                delete_item(to_delete);

                // Optional snippet - calls when deleting an item
                m_print.lock(); // Lock print mutex to prevent jumbling
                std::cout<<"(e) - Removing an item from queue"<<std::endl;
                m_print.unlock();
            }
        }

};

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

    return 0; // Completed successfully

}