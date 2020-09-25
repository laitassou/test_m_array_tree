
/* recursive search
 * limitation : stack  size, depth of tree
 */

#ifndef _RECURSIVE_SEARCH_
#define _RECURSIVE_SEARCH_

#include <iostream>
#include <string>

#include <stack>
#include <map>

#include <vector>


#include "cpplog.hpp"


using namespace cpplog;
using namespace std;

using std::cout;
using std::endl;

namespace recursive_search_insert {


StdErrLogger _log;
		
// decl
template<class T>
bool subtree_search(const tree<T>& t, typename tree<T>::iterator iRoot, int n, T a[]);

/*
 * recursive version of search
 */
template <class T> bool search_word_recursive(const tree<T>& t, int n, T a[])
{

	auto found = false;
	for(typename tree<T>::sibling_iterator iRoots = t.begin(); iRoots != t.end(); ++iRoots)
	{
		if(*iRoots == a[0])
		{
			LOG_INFO(_log)  <<"root found :" << n << "," <<t.number_of_children(iRoots) << "," <<*iRoots<< endl;
			if (n == 1 && t.number_of_children(iRoots) == 0)
			{
				//str << *iRoot;
				found = true;	
			}
			else 
			{
				found = subtree_search(t, iRoots, n-1, a+1);
			}

			if(found)
			{
				break;
			}
		}
	}
	return found;
}


template<class T>
bool subtree_search(const tree<T>& t, typename tree<T>::iterator iRoot, int n, T a[])
{
	auto status = false;
	LOG_DEBUG(_log)  << "root:" <<*iRoot<< ",a0: " << a[0] <<",n:" <<n<< endl;

	// child1, ..., childn
	typename tree<T>::sibling_iterator iChildren;
	for (iChildren = t.begin(iRoot); iChildren != t.end(iRoot); ++iChildren)
	{
		LOG_DEBUG(_log) <<"child :"<< *iChildren << endl;
		if(*iChildren == a[0])
		{
			if(t.number_of_children(iChildren) == 0)
			{
				// match exactly word
				if(n == 1)
				{
					status = true;
				}
				// break anyway
				break;
			}
			else
			{ 
				status = subtree_search(t,iChildren,n-1, a+1);
				if(status)
				{ 
					break;
				}
			}
		}
	}
	return status;

}

/*
* non recursive search, search using depth first iterator  
*/
template <class T> bool search_word_depth(const tree<T>& t, int word_length, T word[])
{
	auto status {false};

	typename tree<T>::sibling_iterator current_root, root = t.begin(); 

	auto current_depth = 0;

	while (root != t.end())
	{
		if( status == true)
		{
			break;
		}

		current_depth = t.depth(root);
		// go no next element in same level [sibling iterator] if current one doesn't match
		if(*root != word[current_depth]) 
		{
			root++;
			continue;
		}

		// if it matchs then go in next depth
		current_root = t.begin(root);
		while(current_root != t.end(root)) 
		{
			current_depth = t.depth(current_root);
			if(*current_root !=  word[current_depth] )
			{
				current_root++;
				continue;
			}

			LOG_DEBUG(_log)  <<"*current_root" <<*current_root << ",current_depth" << current_depth << endl;

			// reached end of branch
			if(t.number_of_children(current_root) == 0  && (current_depth+1) ==word_length )
			{
				status = true;
				break;
			}
			current_root = t.begin(current_root);			
		}
		root++;
	}

	if(status == true)
	{
		LOG_INFO(_log)  <<"word " <<word <<" found" << endl;
	}
	else
	{
		LOG_INFO(_log)  <<"word " <<word <<" not found" << endl;
	}
	return status;

}

/*
* insert to tree
**/
template <class T>
void insert_word(tree<T>& t, int n, T input_word[])
{
	auto index = 0;
	auto status = false;
	auto foundRoot = false;
	auto foundEndPattern = false;
	typename tree<T>::iterator matchRoot;

	for(typename tree<T>::sibling_iterator iRoots = t.begin(); iRoots != t.end(); ++iRoots)
	{
		if(*iRoots == input_word[0])
		{
			foundRoot = true;

			matchRoot = iRoots;
			index++;
			LOG_DEBUG(_log) << "root :" <<*iRoots << endl;
			do {
				LOG_INFO(_log) << "matchRoot :" <<*matchRoot << endl;

				typename tree<T>::sibling_iterator iChildren;

				if (t.number_of_children(matchRoot) == 0)
				{	
					foundEndPattern = true;
				}
				else
				{
					for (iChildren = t.begin(matchRoot); iChildren != t.end(matchRoot); ++iChildren)
					{
						LOG_DEBUG(_log) <<"child :"<<  *iChildren << endl;
						if(*iChildren == input_word[index])
						{	
							LOG_DEBUG(_log) <<"child :"<<  *iChildren << "index:"<<index <<"a:"<<input_word[index]<< endl;
							index++;
							matchRoot = iChildren;
							break;
						}
					
					}
				
					if(iChildren == t.end(matchRoot))
						foundEndPattern = true;

					LOG_DEBUG(_log) << "matchRoot :" <<*matchRoot <<",index:" << index <<","<< ",foundEndPattern:" <<foundEndPattern << endl;
				}
			}while(foundEndPattern == false);

			break;
		}
	}
	if(foundRoot == true)
	{
		//auto level =matchRoot;
		// apppend to parent why = > avoid hiding existing words
		// example 
		// we have this word b c d and we would insert bcdef
		// we should obtain this result : char  d is repeated and it must be
		//	b
		//		c
		//			d
		//			d
		//				e
		//					f

		// last char that matchs at end, so add remaining to previous level
		auto i = 0;
		if(t.number_of_children(matchRoot) == 0)
		{
			auto level = t.parent(matchRoot);
			LOG_DEBUG(_log) <<"match "<<*matchRoot <<"," << *level << "," <<index <<endl;
			i = index -1 ;
			for(auto i = index-1 ; i < n; i++)
			{
				level = t.append_child(level, input_word[i]);
			}	

		}
		else  // last char that matchs before , so don't duplicate this char
		{
			auto level = (matchRoot);			
			LOG_DEBUG(_log) <<"match "<<*matchRoot <<"," << *level << "," <<index <<endl;
			for(auto i = index ; i < n; i++)
			{
				level = t.append_child(level, input_word[i]);
			}	
		}		
	}
	else // No common root found
	{
		auto level = t.begin();
		level = t.insert(level, input_word[0]);
		for(auto i = 1 ; i < n; i++)
		{
			level = t.append_child(level, input_word[i]);
		}		
	}

}

/*
* this means search all words that match at least n-nb_allowed_substitute characters
* precondition : word length must be equal to n
* one traversal of tree, traversal in depth
* To do stop when depth of branch > n and go to next branch
*/
template<class T>
bool tree_search_substitute(const tree<T>& t, int n, T target_word[], int nb_allowed_substitute, tree<T>& tfound)
{
	auto status = false;
	auto index = 0;
	std::vector<int> match;
	std::stack<T> _stack;

	typename tree<T>::pre_order_iterator it_depth;
	bool end_reached {false};
	auto current_depth = 0, last_depth = 0;

	auto need_to_match = n - nb_allowed_substitute;

	if(nb_allowed_substitute >= n)
	{
		LOG_WARN(_log) << "Number of allowed substitute:"<< nb_allowed_substitute << " is higher than n :"<<n <<endl;
		return false;
	}

	it_depth = t.begin();
	while(it_depth != t.end())
	{
			current_depth = t.depth(it_depth);
			if(end_reached)
			{
				LOG_DEBUG(_log) << "stack :"<< _stack.size()<< ",match:" << match.size()<< ",cur:"<<current_depth << ",last:"<<last_depth <<endl;
				for(auto u= current_depth; u <= last_depth; u++)
				{
					_stack.pop();
					match.pop_back();
				}
				end_reached = false;
			}
			
			LOG_DEBUG(_log) << *it_depth <<", depth :"<<t.depth(it_depth) << ",index:" <<index<<endl;
			if(*it_depth == target_word[t.depth(it_depth)])
			match.push_back(1);
			else
			match.push_back(0);


			_stack.push(*it_depth);

			if(t.number_of_children(it_depth) == 0)
			{
				auto sum_match = 0;
				for(auto v =match.begin(); v != match.end(); v++)
				{
					sum_match += *v;
				}
				LOG_DEBUG(_log) << "end of branch" <<"match :" << sum_match << "index:" << index <<endl;

				if(t.depth(it_depth)+1 == n && sum_match >= need_to_match)
				{
					LOG_INFO(_log) << "matched "<<endl;
					status = true;
					// print elements in reverse order <stack> => change to deque
					// avoid copy by uisng other container
					for (std::stack<T> dump = _stack; !dump.empty(); dump.pop())
					{
        				LOG_INFO(_log) << dump.top() << endl;
					}
					//TO DO copy _stack elements to tfound
				}
				end_reached = true;
				last_depth = t.depth(it_depth);
				index = 0;
			}
			it_depth++;
			index++;
	}
	return status;
}



template<class T> class Matrix {
private:
    unsigned _row;
    unsigned _col;
    T * _data;

public:
    Matrix(unsigned row, unsigned col) :_row(row),_col(col){

		unsigned size = _row * _col;
		_data = new T[size]();
	};

	~Matrix()
	{
		delete [] _data;
		cout << "dlter" <<endl;
	}

    void printMatrix()
	{
		for(auto i =0; i <_row; i++)
		{
			cout << "[";
			for(auto  j = 0; j < _col; j++)
			{
				cout << _data[i*_col+j] <<" ";
			}
			cout << "]"<< endl;
		}
	}
};


template<class T> bool estimate_cost(std::vector<T> &vec,int word_length, T word[],int nb_allowed_substitute, int nb_allowed_erasure, int nb_allowed_add)
{
	auto allowed_search_length = word_length + nb_allowed_add;
	Matrix<int> _matrix(vec.size(),word_length);
	_matrix.printMatrix();
	return false;
}

/*
* Version to handle add, erasure of chars  and errors
* It handles erasure of n chars and add of m chars
*/
template<class T>
bool tree_search_add_erase(const tree<T>& ref_tree, int word_length, T word[],int nb_allowed_substitute, int nb_allowed_erasure, int nb_allowed_add, tree<T>& tfound)
{
	auto status = false;
	auto done = false;

	typename tree<T>::pre_order_iterator it_depth;
	typename tree<T>::iterator parent = nullptr;

	auto current_depth = 0, last_depth = 0;
	auto nb_erased = 0, nb_added = 0, nb_matched =0;
	auto allowed_search_length = word_length + nb_allowed_add;
	
	std::vector<char> browsed_chars;

	// preconditions
	if(nb_allowed_substitute >= word_length)
	{
		LOG_WARN(_log) << "Number of allowed substitute:"<< nb_allowed_substitute << " is higher than word_length :"<<word_length <<endl;
		return status;
	}

	if(nb_allowed_erasure >= word_length)
	{
		LOG_WARN(_log) << "Number of allowed erasure:"<< nb_allowed_substitute << " is higher than word_length :"<<word_length<<endl;
		return status;
	}

	it_depth = ref_tree.begin();

		typename tree<T>::sibling_iterator _siblings =ref_tree.begin();
		typename tree<T>::iterator next_to_parent =ref_tree.begin();

	while(it_depth != ref_tree.end())
	{

		if(ref_tree.depth(it_depth) == 0)
		{
			parent = nullptr;
			next_to_parent = nullptr;
		}
		else
		{
			parent = ref_tree.parent(it_depth);
			next_to_parent = parent++;
		}
		if(done == false)
		{
			if( parent == nullptr)
			{
				_siblings = ++(ref_tree.begin());
			}
			else
			{
				_siblings = ++(ref_tree.begin(parent));
				LOG_INFO(_log) << ", _siblings :"<< *_siblings  <<endl;

			}
		}

		current_depth = ref_tree.depth(it_depth);			
		LOG_INFO(_log) << *it_depth <<", depth :"<<ref_tree.depth(it_depth) <<endl;
		browsed_chars.push_back(*it_depth);

		if(ref_tree.number_of_children(it_depth) == 0  && ref_tree.depth(it_depth) <= allowed_search_length-1)
		{
			LOG_INFO(_log) << "end of branch and branch depth:" <<ref_tree.depth(it_depth)<<endl;
			LOG_INFO(_log) << "Analysis:" <<endl;	
			//estimate_cost(browsed_chars, word_length, word, nb_allowed_substitute, nb_allowed_erasure, nb_allowed_add);
		}
		else if(ref_tree.depth(it_depth) >= allowed_search_length-1)
		{
			LOG_INFO(_log) << "parent: "<<*(ref_tree.parent(it_depth)) <<  "it_depth:"<< *it_depth<<endl;
			done = true;

			_siblings++;
			if(_siblings != ref_tree.end(parent))
			{
				it_depth = _siblings;
				LOG_INFO(_log) << "not siblings  " <<"it_depth:"<< *it_depth<<endl;
				continue;
			}
			else
			{
				it_depth = next_to_parent;
				done = false;
				LOG_INFO(_log) << "end of siblings  " <<"it_depth:"<< *it_depth<<"depth:" <<ref_tree.depth(it_depth)<<endl;
				continue;
			}
		}

		//_siblings = ref_tree.begin(parent);
		it_depth++;
	}

}

} // end namespace recursive_search_insert

#endif //_RECURSIVE_SEARCH_
