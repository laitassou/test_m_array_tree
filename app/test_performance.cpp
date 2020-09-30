#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <list>
#include <utility>
#include <stdexcept>
#include <ctime>

#include <vector>

#include <chrono>

#include "tree.hh"
#include "tree_util.hh"
#include "recursive_search.hpp"


/*
* test with tree with 150000 elements
* Current test search 14s, this is reduced to 3s
* by disabling flooding traces
*/



using std::cout;
using std::endl;

using namespace kptree;
using namespace recursive_search_insert;


void print_tree(const tree<char>& tr, tree<char>::pre_order_iterator it, tree<char>::pre_order_iterator end)
{
	if(!tr.is_valid(it)) return;
	int rootdepth=tr.depth(it);
	std::cout << "-----" << std::endl;
	while(it!=end) {
		for(auto i=0; i<tr.depth(it)-rootdepth; ++i) 
			std::cout << "  ";
		std::cout << (*it) << std::endl << std::flush;
		++it;
		}
	std::cout << "-----" << std::endl;
}


/*
 * build some random tree with O(100000) elements and test
 * differents methods
 */ 

int main(int argc, char **argv)
{

	constexpr auto N = 26;
	constexpr auto DEPTH = 10;
	constexpr auto WIDTH = 15;

	char frChars [N] = {};
	tree<char> _tree,result;

	auto count = 0;

	// test chars with some random chars
	for (auto i =0; i < N ; i++)
	{
		frChars[i] = 'a'+i;
	}

	for (auto i =0; i < WIDTH ; i++)
	{
		_tree.insert(_tree.begin(), frChars[i]);
		count++;
	}

	std::srand(0); // use current time as seed for random generator	
	auto status = false;
	typename tree<char>::pre_order_iterator it_depth;
	auto current_depth = 0;
	auto count_depth = 0;
	
	std::vector<typename tree<char>::iterator> vec;


	typename tree<char>::iterator _begin= _tree.begin();
	typename tree<char>::iterator _end= _tree.end();

	it_depth = _begin;

	for (auto i = 0; i < DEPTH; i++)
	{

		it_depth = _begin;
		while(it_depth != _end)
		{
			current_depth = _tree.depth(it_depth);			
			//LOG_INFO(_log) << *it_depth <<", depth :"<<current_depth <<",i:" <<i<<endl;


			//if (_tree.number_of_children(it_depth) == 0)
			{
				vec.push_back(it_depth);
			}

			it_depth++;
		}

		for(auto it =vec.begin(); it != vec.end(); it++)
		{
			auto id = std::rand()/((RAND_MAX + 1u)/N);
			{
				auto level = _tree.append_child(*it, frChars[(id) % N]);
				count++;
			}	
		}		
		vec.empty();

	LOG_INFO(_log) <<  "count_depth:" <<count_depth <<endl;
	count_depth++;
	}
	
	char input[] = "uvabertn";

	// test performance search with substitute, add , erase for tree with  O(100000) elements
	// measured search htime is about   4s
	auto start = chrono::steady_clock::now();
	tree_search_subs_add_erase(_tree,8,input,2,1,1,result);
	auto end = chrono::steady_clock::now();

	LOG_WARN(_log) <<"tree with number of elements :"<< count<< endl;

	cout <<"duration in us:" <<chrono::duration_cast<chrono::microseconds> (end - start).count() <<endl;

	/*
	* Test performance search with substitute tree with O(100000) elements
	* easured search time is about 6ms
	*/

	start = chrono::steady_clock::now();
	tree_search_substitute(_tree,8,input,5,result);
	end = chrono::steady_clock::now();

	LOG_WARN(_log) <<"tree with number of elements :"<< count<< endl;

	cout <<"duration in us:" <<chrono::duration_cast<chrono::microseconds> (end - start).count() <<endl;		
	return 0;
}

