#ifndef VEC_H
#define VEC_H

#include <Arduino.h>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>
#include <unordered_set>
#include <set>

namespace Vec
{
   /*
    https://stackoverflow.com/questions/1577475/c-sorting-and-keeping-track-of-indexes
    Sorts a vector with indices idx, such that v would be ordered from small to large.
    E.g. v = 5 4 3 2 0 1, returns: idx = 4 5 3 2 1 0
    Input: vector v of which the sorted indices should be returned
    Output: vector with the sorted indices of v.
    */
    template <typename T> std::vector< size_t > sort_indices(std::vector<T> &v)
    {   
        // initialize original index locations
        std::vector<size_t> idx(v.size());
        std::iota(idx.begin(), idx.end(), 0);

        // sort indexes based on comparing values in v
        // using std::stable_sort instead of std::sort
        // to avoid unnecessary index re-orderings
        // when v contains elements of equal values 
        std::stable_sort(idx.begin(), idx.end(),
        [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});
        return idx;
    }

    /*  
    https://stackoverflow.com/questions/17074324/how-can-i-sort-two-vectors-in-the-same-way-with-criteria-that-uses-only-one-of/46506051
    Sorts vector v according to vector order. That is, if the second index of order contains a five, the fifth index of v will be moved to 
    the second index in the new ordering. v = 1 2 3 4, order = 2 0 3 1 -> v = 3 1 4 2
    */
    template< typename T > void reorder(std::vector<T> &v, std::vector<size_t> &order )  
    {   
        std::vector<bool> done(v.size());
        for (std::size_t i = 0; i < v.size(); ++i)
        {
            if (done[i])
            {
                continue;
            }
            done[i] = true;
            std::size_t prev_j = i;
            std::size_t j = order[i];
            while (i != j)
            {
                std::swap(v[prev_j], v[j]);
                done[j] = true;
                prev_j = j;
                j = order[j];
            }
        }
        
    }

    /*
    https://stackoverflow.com/questions/838384/reorder-vector-using-a-vector-of-indices
    Reorders vector v based on an order vector. a variant which does destroy the reorder vector (filling it with -1's). 
    Supposedly, it is really fast.
    Inputs: iterators for the order vector, and an iterator for the value vector.
    */
    template< typename order_iterator, typename value_iterator >
    void reorder_destructive( order_iterator order_begin, order_iterator order_end, value_iterator v )  {
        typedef typename std::iterator_traits< value_iterator >::value_type value_t;
        typedef typename std::iterator_traits< order_iterator >::value_type index_t;
        typedef typename std::iterator_traits< order_iterator >::difference_type diff_t;
        
        diff_t remaining = order_end - 1 - order_begin;
        for ( index_t s = index_t(); remaining > 0; ++ s ) {
            index_t d = order_begin[s];
            if ( d == (diff_t) -1 ) continue;
            -- remaining;
            value_t temp = v[s];
            for ( index_t d2; d != s; d = d2 ) {
                std::swap( temp, v[d] );
                std::swap( order_begin[d], d2 = (diff_t) -1 );
                -- remaining;
            }
            v[s] = temp;
        }
    }

    /*
    https://stackoverflow.com/questions/39379411/c-remove-elements-that-are-in-one-vector-from-another
    Removes instances from vector a which are present in both vector a and b
    Inputs: vector a and vector b which should be compared
    */
    template<typename T> void remove_intersection(std::vector<T> & a, std::vector<T> & b)
    {
        std::unordered_multiset<int> st;
        st.insert(a.begin(), a.end());
        st.insert(b.begin(), b.end());
        auto predicate = [&st](const int& k){ return st.count(k) > 1; };
        a.erase(remove_if(a.begin(), a.end(), predicate), a.end());
        //remove comment if the intersection should also be removed from b
        //b.erase(remove_if(b.begin(), b.end(), predicate), b.end());
    } 

    /*  
    Removes duplicate members from the vector v
    Input: reference to the vector v from which duplicates should be removed
    Output: the new length of the vector
    */  
    template<typename T>
    size_t RemoveDuplicates(std::vector<T>& v)
    {
        std::set<T> seen;

        auto newEnd = std::remove_if(v.begin(), v.end(), [&seen](const T& value)
        {
            if (seen.find(value) != std::end(seen))
                return true;

            seen.insert(value);
            return false;
        });

        v.erase(newEnd, v.end());

        return v.size();
    }

}; //namespace Vec


#endif