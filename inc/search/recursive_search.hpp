
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
template <class T> bool search_word(const tree<T>& t, int n, T a[])
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
		if(t.number_of_children(matchRoot) == 0)
		{
			auto level = t.parent(matchRoot);
			LOG_DEBUG(_log) <<"match "<<*matchRoot <<"," << *level << "," <<index <<endl;
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

/*
* Version to handle add, erasure of chars  and errors
* we allow erasure of n chars and add of m chars
* What this means: if we need to mach a word target_word that has n chars 
* from these n chars we have nb_allowed_substitute erros => precondition nb_allowed_substitute < n
* in addition we handle nb_allowerd_erasure => precondition  min (found_word_length) = word_length - nb_allowerd_erasure
* in addition we handle nb_allowed_add => precontion max (found_word_length) = word_length + nb_allowed_add
* To give idea,we need search words that could match in some chars in the range of lengths: [word_length -  nb_allowerd_erasure , word_length + nb_allowed_add]
* example in dic we have word addiction,  audition, etc and we have word = dcvn
*/
template<class T>
bool tree_search_add_erase(const tree<T>& t, int word_length, T word[],int nb_allowed_substitute, int nb_allowerd_erasure, int nb_allowed_add, tree<T>& tfound)
{
	auto status = false;
	typename tree<T>::pre_order_iterator it_depth;
	auto current_depth = 0, last_depth = 0;

	auto nb_erased = 0, nb_added = 0, nb_matched =0;
	

	// preconditions
	if(nb_allowed_substitute >= word_length)
	{
		LOG_WARN(_log) << "Number of allowed substitute:"<< nb_allowed_substitute << " is higher than word_length :"<<word_length <<endl;
		return status;
	}

	if(nb_allowerd_erasure >= word_length)
	{
		LOG_WARN(_log) << "Number of allowed erasure:"<< nb_allowed_substitute << " is higher than word_length :"<<word_length<<endl;
		return status;
	}

	if(nb_allowed_add >= word_length)
	{
		LOG_WARN(_log) << "Number of allowed addition :"<< nb_allowed_add << " is higher than word_length :"<<word_length<<endl;
		return status;
	}

	it_depth = t.begin();

	while(it_depth != t.end())
	{
		current_depth = t.depth(it_depth);			
		LOG_INFO(_log) << *it_depth <<", depth :"<<t.depth(it_depth) <<endl;

		if(*it_depth == word[t.depth(it_depth)])
		{
			nb_matched++;	
		}else
		{
			nb_erased++;
		}

		if(t.number_of_children(it_depth) == 0)
		{

			LOG_INFO(_log) << "end of branch" <<  "index:" <<endl;

		}
		it_depth++;
	}


}

} // end namespace recursive_search_insert

#endif //_RECURSIVE_SEARCH_