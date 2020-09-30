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

