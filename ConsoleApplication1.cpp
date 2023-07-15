// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <map>
#include <iostream>
#include <limits>

#include <iostream>

#include <map>
#include <limits>
#include <iterator>
#include <cassert>
#include <vector>

template<typename K, typename V>
class range_map {
	friend void rangeMapTest();
    friend class test_map;
	V m_valBegin;
	std::map<K, V> m_map;
public:
	// constructor associates whole range of K with val
	range_map(V const& val)
		: m_valBegin(val)
	{}

	// Assign value val to range [keyBegin, keyEnd).
	// Overwrite previous values in this range.
	// If !( keyBegin < keyEnd ), this designates an empty range,
	// and assign must do nothing.
	void assign(K const& keyBegin, K const& keyEnd, V const& val) {

        //std::cout << "here " << keyBegin << " " << keyEnd << std::endl;
        // Empty range check, return if the keyBegin is bigger than
        // or equal to keyEnd
        //
        if (!(keyBegin < keyEnd)) {
            return;
        }
        // Current value at keyBegin
        V currentBeginValue = operator[](keyBegin);
        

        // If keyBegin is not present insert in map
        // Using insert_or_assign as we are not sure if keyBegin is last valid key or not
        // We are also not sure Key is DefaultConstructible, so we cannot use operator[]
        // Documentation about insert_or_assign from: https://en.cppreference.com/w/cpp/container/map/insert_or_assign
        // If a key equivalent to k already exists in the container, assigns std::forward<M>(obj) to the mapped_type corresponding to the key k.
        // If the key does not exist, inserts the new value as if by insert, constructing it from value_type(k, std::forward<M>(obj))
        if (!(currentBeginValue == val)) {
            m_map.insert_or_assign(keyBegin, val);
        }

        // Iterate till keyEnd and erase all K,V pairs before keyEnd. This is necessary to
        // preserve canonocity, that is, consecutive map entries must
        // not contain the same value. This will simulate overwrite.
        // After this we will have to insert currentBeginValue at the keyEnd.
        //
        auto next = m_map.upper_bound(keyBegin);
        auto next_it = next;

        while (next != m_map.cend()) {
            if (keyEnd < next->first) {
                break;
            }
            ++next_it;
            // Update the currentBeginValue as keyEnd may span multiple inetrvals
            currentBeginValue = next->second;
            m_map.erase(next);
            next = next_it;
        }

        // Insert currentBeginValue at the keyEnd
        //
        if (!(currentBeginValue == val)) {
            m_map.insert_or_assign(keyEnd, currentBeginValue);
        }
	}

	// look-up of the value associated with key
	V const& operator[](K const& key) const {
		auto it = m_map.upper_bound(key);
		if (it == m_map.begin()) {
			return m_valBegin;
		}
		else {
			return (--it)->second;
		}
	}
};

class test {
private:
    int a;
public:
    test() = delete;
    test(int a0) : a(a0) {}
};

#include <string>
#include <unordered_map>
void printPassFail(std::string rangeType, int index, int passed, int failed) {
    std::cout <<rangeType<<" | test number = " <<index<<" | passed = "<< passed << " | failed = " << failed << "\n";
}

void rangeMapTest() {
    //Initialise m_map
    //range_map m = range_map<int, char>('A');
    range_map<int, char> m('A');

    int passed = 0;
    int failed = 0;

#pragma region EmptyRange
    passed = 0;
    failed = 3;

    m.assign(3, 3, 'B');
    if (m.m_map.count(3) == 0) {
        //passed++;
        //failed--;
        printPassFail("EmptyRange", 1, 1, 0);
    }
    else {
        printPassFail("EmptyRange", 1, 0, 1);
    }

    m.assign(3, 2, 'B');
    if (m.m_map.count(2) == 0) {
        printPassFail("EmptyRange", 2, 1, 0);
    }
    else {
        printPassFail("EmptyRange", 2, 0, 1);
    }

    if (m.m_map.count(3) == 0) {
        printPassFail("EmptyRange", 3, 1, 0);
    }
    else {
        printPassFail("EmptyRange", 3, 0, 1);
    }

    
#pragma endregion

#pragma region TrivialRange
    passed = 0;
    failed = 11;

    m.assign(1, 10, 'B');
    if (m[0] == 'A') {
        printPassFail("TrivialRange", 1, 1, 0);
    }
    else {
        printPassFail("TrivialRange", 1, 0, 1);
    }

    for (int i = 1; i < 10; i++)
    {
        if (m[i] == 'B') {
            printPassFail("TrivialRange", i + 1, 1, 0);
        }
        else {
            printPassFail("TrivialRange", i + 1, 0, 1);
        }
    }
    std::cout << m[10] << std::endl;
    if (m[10] == 'A') {
        printPassFail("TrivialRange", 11, 1, 0);
    }
    else {
        printPassFail("TrivialRange", 11, 0, 1);
    }

    // std::cout << "TrivialRange | " << passed << " | " << failed << "\n";
#pragma endregion

#pragma region TrivialTwoRange
    passed = 0;
    failed = 9;
    for (int i = 0; i <= 10; i++) {
        std::cout << i << "\t" << m[i] << "\n";
    }
    m.assign(1, 3, 'B');
    for (int i = 0; i <= 10; i++) {
        std::cout << i << "\t" << m[i] << "\n";
    }
    m.assign(6, 8, 'C');
    for (int i = 0; i <= 10; i++) {
        std::cout << i << "\t" << m[i] << "\n";
    }

    if (m[0] == 'A') {
        std::cout << "0\n";
        passed++;
        failed--;
    }
    if (m[1] == 'B') {
        std::cout << "1\n";
        passed++;
        failed--;
    }
    if (m[2] == 'B') {
        std::cout << "2\n";
        passed++;
        failed--;
    }
    if (m[3] == 'A') {
        std::cout << "3\n";
        passed++;
        failed--;
    }
    if (m[4] == 'A') {
        std::cout << "4\n";
        passed++;
        failed--;
    }
    if (m[5] == 'A') {
        std::cout << "5\n";
        passed++;
        failed--;
    }
    if (m[6] == 'C') {
        std::cout << "6\n";
        passed++;
        failed--;
    }
    if (m[7] == 'C') {
        std::cout << "7\n";
        passed++;
        failed--;
    }
    if (m[8] == 'A') {
        std::cout << "8\n";
        passed++;
        failed--;
    }
    for (int i = 0; i < 10; i++) {
        std::cout << i << "\t" << m[i] << "\n";
    }
    std::cout << "TrivialTwoRange | " << passed << " | " << failed << "\n";
#pragma endregion

#pragma region OverwriteLowest
    passed = 0;
    failed = 3;

    m.assign(std::numeric_limits<int>::lowest(), 10000, 'B');

    if (m[0] == 'B') {
        passed++;
        failed--;
    }
    if (m[9999] == 'B') {
        passed++;
        failed--;
    }
    if (m[10000] == 'A') {
        passed++;
        failed--;
    }

    std::cout << "OverwriteLowest | " << passed << " | " << failed << "\n";
#pragma endregion

#pragma region MergeTest
    passed = 0;
    failed = 4;

    m.assign(std::numeric_limits<int>::lowest(), 10, 'B');
    m.assign(10, 20, 'B');

    if (m[0] == 'B') {
        std::cout << "0\n";
        passed++;
        failed--;
    }
    if (m[10] == 'B') {
        std::cout << "10\n";
        passed++;
        failed--;
    }
    if (m[19] == 'B') {
        std::cout << "19\n";
        passed++;
        failed--;
    }
    if (m[20] == 'A') {
        std::cout << "20\n";
        passed++;
        failed--;
    }

    std::cout << "Merge | " << passed << " | " << failed << "\n";
#pragma endregion


}

class test_map 
{
private:
    range_map<int, unsigned char> _map;
public:
    //std::vector<unsigned char> m_check;
    std::unordered_map<unsigned char, std::vector<int>> m_check;
    test_map() : _map(16)
    {
        //m_check.resize(1001, std::vector<unsigned char>(3));
    }
    void assign(int first, int last, unsigned char value, int index)
    {
        //test();
        _map.assign(first, last, value);
        m_check[value].push_back(first);
        m_check[value].push_back(last);
        std::cout << (int)_map[first] << " " << (int)_map[last] << " " << (int)value << std::endl;
        if (first < last)
        {
            assert(_map[first] == value);
            if (last - 1 > first)
            {
                assert(_map[last-1] == value);
            }
            
        }
        /*else
        {
            assert
        }*/
        
        
        //test(index);
    }
    void test(int index)
    {
        
        
        
        //auto prev_it = _map.m_map.begin();
        //std::cout << _map.m_map.empty() << std::endl;
        //assert(_map.m_map.empty());
        //assert(prev_it->first == 0);
        /*for (auto it = _map.m_map.begin(); it != _map.m_map.end(); ++it, ++prev_it)
        {
            assert(prev_it->second == it->second);
            for (int i = prev_it->first; i < it->first; ++i)
                assert(m_check[i] == prev_it->second);
        }
        prev_it = _map.m_map.begin();
        if (prev_it != _map.m_map.end())
        {
            for (int i = prev_it->first; prev_it != _map.m_map.end() && i < 256; ++i)
                assert(m_check[i] == prev_it->second);
        }*/
        
    }
};


int main() {
    for (int t = 0; t < 300; ++t)
    {
        test_map tm;
        for (int i = 0; i < 1000; ++i)
        {
            int start = rand() % 256;
            int end = rand() % 256;
            tm.assign(start, end, rand() % 16, i);
        }
        //tm.test();
    }
    std::cout << "Done Test!" << std::endl;
    //rangeMapTest();
    return 0;
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
