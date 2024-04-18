//
// Created by aleks on 07.01.2024.
//

#include <iostream>
#include <fstream>
#include <map>
#include <chrono>
#include "avl_tree_test.h"
void test(int defaultInfo) {
    bool error= false;
    std::cout<<"Testing ctor...";
    avl_tree<int, int> tree(defaultInfo);
    if(!tree.empty()) {
        error= true;
        std::cerr << "Error in empty. Tree should be empty after ctor" << std::endl;
        auto vector = tree.getNodes( 100, [](const std::pair<int,int>& a, const std::pair<int,int>& b){return true;},[](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.second < b.second;});
        std::cerr<<"Nodes in a avlTree:"<< std::endl;
        for (auto & i : vector) {
            std::cout<<"Key: "<<i.first<<" Info: "<<i.second<< std::endl;
        }
    }
    if(defaultInfo != tree.getDefaultInfo()) {
        error=true;
        std::cerr << "Error in default info. Should be " << defaultInfo << " instead of " << tree.getDefaultInfo() << std::endl;
        std::cout << "ctorTest concluded" << std::endl;
    }
    if(!error)std::cout<<" OK"<<std::endl;

    std::cout<<"Testing insert on new element...";
    tree.insert(1, 1, [](const int& a, const int & b){return a + b;});
    if(tree.empty()) {
        std::cerr << "Error in empty. Tree should be not empty after insert" << std::endl;
        error=true;
    }
    auto vector = tree.getNodes(100, [](const std::pair<int,int>& a, const std::pair<int,int>& b){return true;},[](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.second < b.second;});
    if(vector.size() != 1) {
        std::cerr << "Error in getNodes after adding a node with insert. Size of the vector should be: 1 instead of"<< vector.size() << std::endl;
        error=true;
    }
    int a =1;
    for (auto & i : vector) {
        if (i.first!=1) {
            std::cerr << "Error in getNodes. Key of the " << a << " element should be: 1 instead of" << i.first<< std::endl;
            error=true;
        }
        if (i.second!=1) {
            std::cerr << "Error in getNodes. Info of the " << a << " element should be: 1 instead of" << i.second<< std::endl;
            error=true;
        }
        a++;
    }
    if(!error)std::cout<<" OK"<<std::endl;
    error= false;

    std::cout<<"Testing insert on existing element...";
    tree.insert(1, 2, [](const int& a, const int & b){return a + b;});
    vector = tree.getNodes(100, [](const std::pair<int,int>& a, const std::pair<int,int>& b){return true;},[](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.second < b.second;});
    if(vector.size() != 1) {
        std::cerr<< "Error in getNodes after updating existing node with insert. Size of the vector should be: 1 instead of"<< vector.size() << std::endl;
        error=true;
    }
    a=1;
    for (auto & i : vector) {
        if (i.first!=1) {
            std::cerr << "Error in getNodes. Key of the " << a << " element should be: 1 instead of" << i.first<< std::endl;
            error=true;
        }
        if (i.second!=3) {
            std::cerr << "Error in getNodes. Info of the " << a << " element should be: 3 instead of" << i.second<< std::endl;
            error=true;
        }
        a++;
    }
    if(!error)std::cout<<" OK"<<std::endl;
    error=false;

    std::cout<<"Testing insert for more elements...";
    tree.insert(1, 2, [](const int& a, const int & b){return a - b;});
    for (int i = 2; i < 5; ++i) {
        tree.insert(i, i, [](const int& a, const int & b){return a + b;});
    }
    vector = tree.getNodes(100, [](const std::pair<int,int>& a, const std::pair<int,int>& b){return true;},[](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.second < b.second;});
    if(vector.size() != 4) {
        std::cerr << "Error in getNodes after adding 5 nodes with insert. Size of the vector should be: 5 instead of"<< vector.size() << std::endl;
        error= true;
    }
    for (int i=1; i < vector.size(); ++i) {
        if (vector[i - 1].first != i) {
            std::cerr << "Error in getNodes. Key of the " << a << " element should be: " << i << " instead of"<< vector[i - 1].first << std::endl;
            error=true;
        }

        if (vector[i - 1].second != i) {
            std::cerr << "Error in getNodes. Info of the " << a << " element should be: " << i << " instead of"<< vector[i - 1].second << std::endl;
            error=true;
        }
        a++;
    }
    if(!error)std::cout<<" OK"<<std::endl;
    error=false;

    std::cout<<"Testing pop on a non existent element...";
    tree.pop(-1);
    vector = tree.getNodes(100, [](const std::pair<int,int>& a, const std::pair<int,int>& b){return true;},[](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.second < b.second;});
    if(vector.size() != 4) {
        std::cerr << "Error in getNodes after adding 5 nodes with insert. Size of the vector should be: 4 instead of"<< vector.size() << std::endl;
        error= true;
    }
    for (int i=1; i < vector.size(); ++i) {
        if (vector[i - 1].first != i) {
            std::cerr << "Error in getNodes. Key of the " << a << " element should be: " << i << " instead of"<< vector[i - 1].first << std::endl;
            error=true;
        }
        if (vector[i - 1].second != i) {
            std::cerr << "Error in getNodes. Info of the " << a << " element should be: " << i << " instead of"<< vector[i - 1].second << std::endl;
            error= true;
        }
        a++;
    }
    if(!error)std::cout<<" OK"<<std::endl;
    error=false;

    std::cout<<"Testing pop for element on the end of the avlTree...";
    tree.pop(4);
    vector = tree.getNodes(100, [](const std::pair<int,int>& a, const std::pair<int,int>& b){return true;},[](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.second < b.second;});
    if(vector.size() != 3) {
        std::cerr << "Error in getNodes after adding 5 nodes with insert. Size of the vector should be: 3 instead of"<< vector.size() << std::endl;
        error=true;
    }
    for (int i=1; i < vector.size(); ++i) {
        if (vector[i - 1].first != i) {
            std::cerr << "Error in getNodes. Key of the " << a << " element should be: " << i << " instead of"<< vector[i - 1].first << std::endl;
            error=true;
        }
        if (vector[i - 1].second != i) {
            std::cerr << "Error in getNodes. Info of the " << a << " element should be: " << i << " instead of"<< vector[i - 1].second << std::endl;
            error=true;
        }
        a++;
    }
    if(!error)std::cout<<" OK"<<std::endl;
    error=false;

    std::cout<<"Testing pop for an element inside a avlTree...";
    tree.pop(2);
    vector = tree.getNodes(100, [](const std::pair<int,int>& a, const std::pair<int,int>& b){return true;},[](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.second < b.second;});
    if(vector.size() != 2) {
        std::cerr << "Error in getNodes after adding 5 nodes with insert. Size of the vector should be: 2 instead of"<< vector.size() << std::endl;
        error=true;
    }
    for (int i=0; i < vector.size(); ++i) {
        if (vector[i].first != i + 1 + (i == 1 ? 1 : 0)) {
            std::cerr << "Error in getNodes. Key of the " << i << " element should be: " << 1 + i + (i == 1 ? 1 : 0)<< " instead of " << vector[i].first << std::endl;
            error=true;
        }
        if (vector[i].second != i + 1 + (i == 1 ? 1 : 0)) {
            std::cerr << "Error in getNodes. Info of the " << i << " element should be: " << 1 + i + (i == 1 ? 1 : 0)<< " instead of " << vector[i].second << std::endl;
            error=true;
        }
    }
    if(!error)std::cout<<" OK"<<std::endl;
    error=false;

    std::cout<<"Testing clear...";
    tree.clear();
    if(!tree.empty()) {
        error=true;
        vector = tree.getNodes(100, [](const std::pair<int,int>& a, const std::pair<int,int>& b){return true;},[](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.second < b.second;});
        std::cerr << "Error in clear. Tree should be empty, and contains " << vector.size() << " elements instead." << std::endl;
        std::cerr<<"Elements:"<<std::endl;
        for (auto & it:vector) {
            std::cerr<<"Key: "<<it.first<<" Info: "<<it.second<<std::endl;
        }
    }
    if(!error)std::cout<<" OK"<<std::endl;

    std::cout<<"Testing clear on an empty avlTree...";
    tree.clear();
    std::cout<<" OK"<<std::endl;
    error=false;

    std::cout<<"Testing cctor and operator=...";
    for (int i = 0; i < 5; ++i) {
        tree.insert(i, i, [](const int& a, const int & b){return a + b;});
    }
    avl_tree<int, int> tree2(tree);
    if(tree2.empty()) {
        std::cerr << "Error in cctor. Tree should not be empty" << std::endl;
        error=true;
    }
    vector = tree2.getNodes(100, [](const std::pair<int,int>& a, const std::pair<int,int>& b){return true;},[](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.second < b.second;});
    if (vector.size()!=5){
        std::cerr<<"Error in cctor. Tree should consist of 5 elements, instead of "<<vector.size() << std::endl;
        error=true;
    }
    for (int i=0; i < vector.size(); ++i) {
        if (vector[i].first != i  ) {
            std::cerr << "Error in getNodes. Key of the " << i << " element should be: " << i<< " instead of " << vector[i].first << std::endl;
            error=true;
        }
        if (vector[i].second != i ) {
            std::cerr << "Error in getNodes. Info of the " << i << " element should be: " << i<< " instead of " << vector[i].second << std::endl;
            error=true;
        }
    }
    if(!error)std::cout<<" OK"<<std::endl;

    std::cout << "Testing forEach...";
    tree.clear();
    for (int i = 1; i <= 5; ++i) {
        tree.insert(i, i, [](const int& a, const int& b) { return a + b; });
    }
    auto operation = [](int& oldInfo, int& newInfo) {
        oldInfo = newInfo * 2; // Modify the oldInfo using newInfo
    };
    int newInfo = 10;
    tree.forEach(newInfo, operation);
    vector = tree.getNodes(100, [](const std::pair<int,int>& a, const std::pair<int,int>& b){return true;},[](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.second < b.second;});
    a = 1;
    for (const auto& node : vector) {
        if (node.second != 20) {
            std::cerr << "Error in forEach. Info of the " << a << " element should be: " << 20 << " instead of " << node.second << std::endl;
            error = true;
        }
        a++;
    }
    if (!error) {
        std::cout << " OK" << std::endl;
    }
    error=false;

    std::cout<<"Testing operator[]...";
    for (int i = 1; i <= 5; ++i) {
        tree[i];
    }
    vector = tree.getNodes(100, [](const std::pair<int,int>& a, const std::pair<int,int>& b){return true;},[](const std::pair<int,int>& a, const std::pair<int,int>& b) {return a.second < b.second;});
    a = 1;
    for (const auto& node : vector) {
        if (node.second != 21) {
            std::cerr << "Error in forEach. Info of the " << a << " element should be: " << 20 << " instead of " << node.second << std::endl;
            error = true;
        }
        a++;
    }
    if (!error) {
        std::cout << " OK" << std::endl;
    }


    std::cout<<"Test concluded"<<std::endl;
}
void testMaxInfoSelector() {
    std::cout << "Testing maxinfo_selector..." << std::endl;

    // Creating an AVL tree with some nodes
    avl_tree<int, int> tree(-1);
    for (int i = 1; i <= 5; ++i) {
        tree.insert(i, i * 10, [](const int& a, const int& b) { return a + b; });
    }

    // Testing maxinfo_selector with a count of 3
    std::cout << "Testing maxinfo_selector with a count of 3...";
    std::vector<std::pair<int, int>> result = maxinfo_selector(tree, 3);

    // Expected values
    std::vector<std::pair<int, int>> expected = {{3, 30}, {4, 40}, {5, 50}};

    // Check if the result matches the expected values
    if (result == expected) {
        std::cout << "OK" << std::endl;
    } else {
        std::cerr << "Error in maxinfo_selector. Result does not match expected values." << std::endl;
        for (size_t i = 0; i < result.size(); ++i) {
            if (i < expected.size() && result[i] != expected[i]) {
                std::cerr << "Mismatch at index " << i << ". Expected Key: " << expected[i].first
                          << " Info: " << expected[i].second << ", Actual Key: " << result[i].first
                          << " Info: " << result[i].second << std::endl;
            } else if (i >= expected.size()) {
                std::cerr << "Extra node in result at index " << i << ". Key: " << result[i].first
                          << " Info: " << result[i].second << std::endl;
            }
        }
    }

    // Testing maxinfo_selector with a count larger than the number of nodes
    std::cout << "Testing maxinfo_selector with a count larger than the number of nodes...";
    result = maxinfo_selector(tree, 10);

    // Expected values
    expected = {{1, 10}, {2, 20}, {3, 30}, {4, 40}, {5, 50}};

    // Check if the result matches the expected values
    if (result == expected) {
        std::cout << "OK" << std::endl;
    } else {
        std::cerr << "Error in maxinfo_selector. Result does not match expected values." << std::endl;
        for (size_t i = 0; i < result.size(); ++i) {
            if (i < expected.size() && result[i] != expected[i]) {
                std::cerr << "Mismatch at index " << i << ". Expected Key: " << expected[i].first
                          << " Info: " << expected[i].second << ", Actual Key: " << result[i].first
                          << " Info: " << result[i].second << std::endl;
            } else if (i >= expected.size()) {
                std::cerr << "Extra node in result at index " << i << ". Key: " << result[i].first
                          << " Info: " << result[i].second << std::endl;
            }
        }
    }

    std::cout << "maxinfo_selector test concluded" << std::endl;
}
void minusTest(){
    avl_tree<int, int> tree(-1);
    avl_tree<int, int> tree2(-1);
    for (int i = 0; i < 5; ++i) {
        tree.insert(i,i,[](const int& a, const int& b){return a+b;});
        tree2.insert(2*i,i,[](const int& a, const int& b){return a+b;});
    }

    avl_tree<int,int> tree3 = tree.subtract(tree2);
    std::cout<<std::endl;
}
void speedTest() {
    const int num_reps = 1;
    std::chrono::milliseconds map_total_time(0);
    std::chrono::milliseconds my_total_time(0);

    // std::map
    std::cout << "std::map:\n";
    for (int rep = 0; rep < num_reps; ++rep) {
        std::ifstream is("beagle_voyage.txt");
        if (!is) {
            std::cout << "Error opening input file." << std::endl;
            return;
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        std::string word;
        std::map<std::string, int> wc;
        while (is >> word) wc[word]++;
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time =  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        map_total_time += time;
    }

    // Calculate and print average for std::map
    std::chrono::milliseconds map_average_time = map_total_time / num_reps;
    std::cout << "Average time (std::map): " << map_average_time.count() << " ms.\n";

    // Your implementation (avl_tree)
    std::cout << std::endl << "Mine:\n";
    for (int rep = 0; rep < num_reps; ++rep) {
        std::ifstream is("beagle_voyage.txt");
        if (!is) {
            std::cout << "Error opening input file." << std::endl;
            return;
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        std::string word;
        avl_tree<std::string, int> wc(1);
        while (is >> word) wc[word];
        auto end_time = std::chrono::high_resolution_clock::now();

        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        my_total_time += time;
    }

    // Calculate and print average for your implementation
    std::chrono::milliseconds my_average_time = my_total_time / num_reps;
    std::cout << "Average time (Mine): " << my_average_time.count() << " ms.\n";

    // Print the ratio of your average time to std::map average time
    std::cout << "Ratio (Mine / std::map): " << static_cast<double>(my_average_time.count()) / map_average_time.count() << std::endl;
    //My code usually takes 75% of the map time
}

void subtractStressTest(){
    std::ifstream is("beagle_voyage.txt");
    if (!is) {
        std::cout << "Error opening input file." << std::endl;
        return;
    }
    std::string word;
    avl_tree<std::string, int> wc(1);
    while (is >> word) wc[word];
    std::ifstream is2("beagle2.txt");
    if (!is2) {
        std::cout << "Error opening input file." << std::endl;
        return;
    }
    avl_tree<std::string, int> wc2(1);
    while (is2 >> word) wc2[word];
    wc.subtract(wc2);
std::cout<<"done";
}



