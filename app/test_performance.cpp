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
	char frChars [N] = {};
	tree<char> largeTree;

	auto count = 0;

	// test chars with some random chars
	for (auto i =0; i < N ; i++)
	{
		frChars[i] = 'a'+i;
	}

	for (auto i =0; i < N ; i++)
	{
		largeTree.insert(largeTree.begin(), frChars[i]);

	}

	std::srand(0); // use current time as seed for random generator	
	for(tree<char>::sibling_iterator iRoots = largeTree.begin(); iRoots != largeTree.end(); ++iRoots)
	{
		auto id = std::rand()/((RAND_MAX + 1u)/N);
		auto depth = std::rand()/((RAND_MAX + 1u)/DEPTH) + 1;
		typename tree<char>::iterator levels[DEPTH] = {};

		cout << "depth :" << depth<< endl;
		auto level = largeTree.append_child(iRoots,frChars[id]);
		count++;
		auto i =0;

		while(i < depth)
		{
			id = std::rand()/((RAND_MAX + 1u)/N);
			 levels[i] = level;
			for(auto j = 0; j < 9; j++)
			{
				id = std::rand()/((RAND_MAX + 1u)/N);
		 		level = largeTree.append_child(level,frChars[id]);
				count++;
			}	
			level = levels[i];
			i++;
		}
	}

	LOG_INFO(_log) <<"tree with number of elements :"<< count<< endl;
	print_tree(largeTree, largeTree.begin(), largeTree.end());

	return 0;
}

