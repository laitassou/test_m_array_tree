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

int main(int argc, char **argv)
{


	constexpr auto N = 26;
	char frChars [N] = {};
	tree<char> largeTree;
	tree<char> result;

	// test chars
	for (auto i =0; i < N ; i++)
	{
		frChars[i] = 'a'+i;
	}

	for (auto i =0; i < 3 ; i++)
	{
		largeTree.insert(largeTree.begin(), frChars[i]);
	}


	std::srand(0); // use current time as seed for random generator
	
	for(tree<char>::sibling_iterator iRoots = largeTree.begin(); iRoots != largeTree.end(); ++iRoots)
	{

		auto random_variable = std::rand();
		auto id = std::rand()/((RAND_MAX + 1u)/N);

		for(auto i =1; i <3;i++)
		{
			auto level = largeTree.append_child(iRoots, frChars[(id+i) % N]);

			for(auto j =1; j <2; j++)
			{
				largeTree.append_child(level, frChars[(id+j) % N]);
			}
		}


	}
	LOG_DEBUG(_log) <<" 1 level "<<endl;
	//print_tree_bracketed(largeTree);
	print_tree(largeTree, largeTree.begin(), largeTree.end());


	char input_3[] = "bxv";
	LOG_INFO(_log) <<"  search word: "<<input_3 <<endl;

	tree_search_substitute(largeTree,3,input_3,1,result);

	return 0;
}
