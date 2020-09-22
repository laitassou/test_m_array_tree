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

void print_tree(const tree<std::string>& tr, tree<std::string>::pre_order_iterator it, tree<std::string>::pre_order_iterator end)
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

	tree<char> largeTree;

	char input_1 [] = "abc";
	char input_2 [] = "bcd";
	char input_3 [] = "bcdef";
	char input_4 [] = "bcefxyz";

	LOG_DEBUG(_log) << input_1 << endl;

	print_tree(largeTree, largeTree.begin(), largeTree.end());
	insert_word(largeTree,3, input_1);
	print_tree(largeTree, largeTree.begin(), largeTree.end());

	/* insert in new root */
	insert_word(largeTree,3, input_2);
	print_tree(largeTree, largeTree.begin(), largeTree.end());

	/* insert with leaf match */

	insert_word(largeTree,5, input_3);

	print_tree(largeTree, largeTree.begin(), largeTree.end());

	/* insert with middle match */
	insert_word(largeTree,7, input_4);
	print_tree(largeTree, largeTree.begin(), largeTree.end());


	// add test by checking if words exist
	return 0;

}

