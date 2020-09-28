/* 

	STL-like templated tree class; test program.
	Copyright (C) 2001-2009  Kasper Peeters <kasper.peeters@aei.mpg.de>

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
*/

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
	constexpr auto WIDTH = 26;

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
			LOG_INFO(_log) << *it_depth <<", depth :"<<current_depth <<",i:" <<i<<endl;


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
	
	LOG_INFO(_log) <<"tree with number of elements :"<< count<< endl;
	print_tree(_tree, _tree.begin(), _tree.end());
	LOG_INFO(_log) <<"tree with number of elements :"<< count<< endl;

	char input[] = "uvabertn";

	auto start = chrono::steady_clock::now();
	tree_search_add_erase(_tree,8,input,2,1,1,result);
	auto end = chrono::steady_clock::now();

	cout <<chrono::duration_cast<chrono::microseconds> (end - start).count() <<endl;	
	return 0;
}

