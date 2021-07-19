
// vl_vector.h

#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_
#include <cmath>
#include <algorithm>
#include <stdexcept>
#define OUT_OF_RANGE "Error: Index out of range"
#define DF_CAPACITY 16
enum {
    ONE = 1, TWO, THREE
};

template<typename T, size_t StaticCapacity = DF_CAPACITY>
class vl_vector {
 public:
  /**
   * default constructor - inits an empty vector, contains static array with
   * size of the given capacity.
   */
  vl_vector () : _cur_capacity (StaticCapacity), _size (0),
                 _dynamic_data (nullptr)
  {}

  /**
   * copy constructor - copy all members from other vector. if the data of
   * the other vector is static- make it static, and if dynamic - allocate
   * a dynamic array.
   * @param other - the vector to copy from
   */
  vl_vector (const vl_vector &other)
  {
    _cur_capacity = other._cur_capacity;
    _size = other._size;
    if (_cur_capacity > StaticCapacity) // if other's data is dynamic -
      // allocate new array to the dynamic data.
      {
        _dynamic_data = new T[_cur_capacity];
        std::copy (other.begin (), other.end (), _dynamic_data);

      }
    else // if other's data is static - only copy to the static array.
      {
        std::copy (other.begin (), other.end (), _static_data);
        _dynamic_data = nullptr;
      }
  }
  /**
   * inits vector with data that contains the values from the given sequence.
   * @tparam ForwardIterator - the type of the iterators we receive
   * @param first iterator to the first element in the sequence
   * @param last iterator to the one after the last element in the sequence
   */
  template<typename ForwardIterator>
  vl_vector (ForwardIterator first, ForwardIterator last)
  {
    size_t i = std::distance (first, last); // count how many items we need to
    // insert
    _size = 0;
    _cur_capacity = StaticCapacity;
    _cur_capacity = calculate_capacity (i); // inits the new capacity after
    // the insertion.
    _size = i;
    if (_cur_capacity <= StaticCapacity) // we need static array
      {
        std::copy (first, last, _static_data);
        _dynamic_data = nullptr;
      }
    else // we need dynamic array
      {
        _dynamic_data = new T[_cur_capacity];
        std::copy (first, last, _dynamic_data);
      }
  }

  /**
   * inits new vector with n elements from the given value
   * @param n number of elements to insert
   * @param val the value to insert
   */
  vl_vector (size_t n, const T &val)
  {
    _size = 0;
    _cur_capacity = StaticCapacity;
    _cur_capacity = calculate_capacity (n); // inits the new capacity after
    // the insertion.
    _size = n;
    if (_cur_capacity > StaticCapacity) // case of dynamic data
      {
        _dynamic_data = new T[_cur_capacity];
        std::fill_n (_dynamic_data, n, val);
      }
    else // case of static data
      {
        _dynamic_data = nullptr;
        std::fill_n (_static_data, n, val);
      }
  }

  /**
* destructor.
* deleting the dynamic memory.
*/
  ~vl_vector ()
  {
    if (_cur_capacity > StaticCapacity)
      {
        delete[] _dynamic_data;
      }
  }

  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  /**
   * @return number of elements in the vector
   */
  size_t size () const
  {
    return _size;
  }
  /**
   * @return the current capacity of the vector
   */
  size_t capacity () const
  {
    return _cur_capacity;
  }
  /**
   * @return true if the vector is empty, false else
   */
  bool empty () const
  {
    return (_size == 0);
  }
  /**
   * non const version of at method - return reference to the value in the
   * i'th slot, if exist
   * @param i - the index of the asked element
   * @return reference to the i'th element
   */
  T &at (size_t i)
  {
    if (i < 0 || i >= _size) // check validity
      {
        throw std::out_of_range (OUT_OF_RANGE);
      }
    else
      {
        if (_cur_capacity > StaticCapacity) // if the current array is the
          // dynamic
          {
            return _dynamic_data[i];
          }
        else  // if the current array is the static
          {
            return _static_data[i];
          }
      }

  }

  /**
   * const version of at method - return copy of the value in the
   * i'th slot, if exist
   * @param i - the index of the asked element
   * @return const reference to the i'th element
   */
  const T &at (size_t i) const
  {
    if (i < 0 || i >= _size) // check validity
      {
        throw std::out_of_range (OUT_OF_RANGE);
      }
    else
      {
        if (_cur_capacity > StaticCapacity) // if the current array is the
          // dynamic
          {
            return _dynamic_data[i];
          }
        else  // if the current array is the static
          {
            return _static_data[i];
          }
      }
  }
  /**
   * insert value to the end of the vector. change data and capacity,
   * if need to.
   * @param new_val the new value to insert
   */
  void push_back (const T &new_val)
  {
    size_t tmp_cap = calculate_capacity (ONE);
    if (tmp_cap == _cur_capacity) // dont need to change the data
      {
        std::fill_n (end (), ONE, new_val);
        _size++;
      }
    else // need to change the data size
      {
        T *tmp_data = new T[tmp_cap];
        std::copy (begin (), end (), tmp_data);
        tmp_data[_size++] = new_val;
        if (_cur_capacity == StaticCapacity) // move from static to dynamic
          {
            _dynamic_data = tmp_data;
          }
        else // move from dynamic to static
          {
            delete[] _dynamic_data; // delete the old data
            _dynamic_data = tmp_data;
          }
      }
    _cur_capacity = tmp_cap;
  }

  /**
   * inserting new element before the element at the specified position,
   * use sequence insert function - sequence with one element
   * @param pos Position in the vector where the new element is inserted.
   * @param val Value to be copied to the inserted element
   * @return An iterator that points to the new inserted element.
   */
  iterator insert (const_iterator pos, const T &val)
  {
    T arr[TWO];
    arr[0] = val;
    arr[1] = val;
    return insert (pos, arr, arr + 1);
  }

  /**
  * inserting new sequence before the element at the specified position,
  * use two helper functions that insert the value according to
  * the current data situation.
  * @param pos Position in the vector where the new element is inserted.
  * @param val Value to be copied to the inserted element
  * @return An iterator that points to the new inserted element.
  */
  template<typename ForwardIterator>
  iterator insert (const_iterator pos, ForwardIterator first,
                   ForwardIterator last)
  {
    size_t n = std::distance (first, last); // number of elements to insert
    size_t tmp_cap = calculate_capacity (n);
    iterator final_it;
    if (tmp_cap == _cur_capacity) // don't need to change the array
      {
        insert_copy_data_same_array (n, first, last, pos, final_it);
      }
    else // need to change the array
      {
        if (_cur_capacity <= StaticCapacity) // move from static to dynamic
          {
            _dynamic_data = new T[tmp_cap];
            insert_copy_data_other_array (n, first, last, pos, _dynamic_data,
                                          final_it);
          }
        else // move from dynamic to dynamic
          {
            T *tmp_data = new T[tmp_cap];
            insert_copy_data_other_array (n, first, last, pos, tmp_data,
                                          final_it);
            delete[] _dynamic_data;
            _dynamic_data = tmp_data;
          }
      }
    _cur_capacity = tmp_cap;
    return final_it;
  }

  /**
   * Pop the last element from the vector. When size > StaticCapacity and
   * size - 1 <= StaticCapacity, copies all data to the static data.
   * Otherwise, stay in the current data.
   * Update the size of the vector.
   */
  void pop_back ()
  {
    if (_size == 0)
      {
        return;
      }
    if (_size - ONE > StaticCapacity || _size <= StaticCapacity)
      {
        _size -= ONE;
      }
    else
      {
        std::copy (begin (), end () - ONE, _static_data);
        _size -= ONE;
        _cur_capacity = StaticCapacity;
        delete[] _dynamic_data;
        _dynamic_data = nullptr;
      }
  }
  /**
   * Removes the element at pos. change the capacity and the data memory
   * if needed.
   * @param pos the element to remove
   * @return iterator following the removed element
   */
  iterator erase (const_iterator pos)
  {
    if (pos == this->end () - ONE)
      {
        pop_back ();
        return end ();
      }
    return erase (pos, pos + ONE);
  }

  /**
   * Removes the elements in the range [first, last).
   * @param first the start of the range of elements to remove
   * @param last the end of the range of elements to remove
   * @return iterator following the last removed element
   */
  iterator erase (const_iterator first, const_iterator last)
  {
    iterator final_it; // represents the iterator following the last removed
    // element
    size_t n = std::distance (first, last);
    if (n == 0)
      {
        return begin () + std::distance (cbegin (), last);
      }

    size_t first_place = std::distance (cbegin (), first);
    size_t last_place = std::distance (cbegin (), last);
    if (_size - n > StaticCapacity || _size <= StaticCapacity) // there no need
      // to change the data or capacity
      {
        std::move (begin () + last_place, end (), begin () + first_place);
        final_it = begin () + first_place;
      }
    else // need to change the data to static array
      {
        std::move (begin (),
                   begin () + first_place, _static_data); // copies the
        // beginning of the vector (up to 'first') to the static array.
        std::move (begin () + last_place, end (), _static_data + first_place);
        // copies the 'last' element and forward to the static array
        _cur_capacity = StaticCapacity;
        delete[] _dynamic_data;
        _dynamic_data = nullptr;
        final_it = _static_data + first_place;
      }
    _size = _size - n;
    return final_it;
  }
  /**
   * clear the vector - delete dynamic data if there is need, inits size = 0
   * and capacity to the static capacity
   */
  void clear ()
  {
    if (_cur_capacity > StaticCapacity)
      {
        delete[] _dynamic_data;
        _dynamic_data = nullptr;
      }
    _size = 0;
    _cur_capacity = StaticCapacity;
  }

  /**
   * check if the given value is in the vector
   * @param val - the value to find in the vector
   * @return true is the contains val, false else.
   */
  bool contains (const T &val) const
  {
    const_iterator it = std::find (begin (), end (), val);
    return (it != cend ());
  }

  /**
   * @return pointer to the current active array - dynamic or static
   */
  T *data ()
  {
    if (_cur_capacity > StaticCapacity)
      {
        return _dynamic_data;
      }
    return _static_data;
  }
  /**
  * @return pointer to the current active array - dynamic or static.
   * const version
  */
  const T *data () const
  {
    if (_cur_capacity > StaticCapacity)
      {
        return _dynamic_data;
      }
    return _static_data;
  }

  /**
   * non const [] operator.
   * return the element from the current data array, according to
   * the capacity situation. Does not throw an exception if index out of range.
   * @param i the index of the wanted element
   * @return reference to the element in the given index
   */
  T &operator[] (size_t i)
  {
    if (_cur_capacity <= StaticCapacity)
      {
        return _static_data[i];
      }
    return _dynamic_data[i];
  }
  /**
   * const [] operator.
   * @param i the index of the wanted element
   * @return the element in the given index
   */
  T operator[] (size_t i) const
  {
    if (_cur_capacity <= StaticCapacity)
      {
        return _static_data[i];
      }
    return _dynamic_data[i];
  }

  /**
   * opy assignment operator. Replaces the contents with a copy of the
   * contents of other.
   * @param other another vector to use as data source
   * @return reference to this
   */
  vl_vector &operator= (const vl_vector &other)
  {
    if (this == &other) // self- assignment situation
      {
        return *this;
      }
    if (_cur_capacity > StaticCapacity) // if this has a dynamic data - delete
      {
        delete[] _dynamic_data;
        _dynamic_data = nullptr;
      }
    _size = other._size;
    _cur_capacity = other._cur_capacity;
    if (_cur_capacity > StaticCapacity) // if need dynamic data
      {
        _dynamic_data = new T[_cur_capacity];
        std::copy (other.cbegin (), other.cend (), _dynamic_data);
      }
    else // if need static data
      {
        std::copy (other.cbegin (), other.cend (), _static_data);
      }
    return *this;
  }

  /**
   * operator ==. return true only if the size and the data contacts are equals
   * @param other other vector to compare with
   * @return true if equals, false else
   */
  bool operator== (const vl_vector &other) const
  {
    if (_size != other._size)
      { return false; }
    return std::equal (begin (), end (), other.begin ());
  }

  /**
  * operator != , use operator ==
  * @param other other vector to compare with
  * @return false if equals, false else
  */
  bool operator!= (const vl_vector &other) const
  {
    return !((*this) == other);
  }

  /**
   * @return begin iterator
   */
  iterator begin ()
  {
    return data ();
  }
  /**
  * @return end iterator
  */
  iterator end ()
  {
    return data () + _size;
  }
  /**
  * @return const version of begin iterator
  */
  const_iterator begin () const
  {
    return data ();
  }

  /**
  * @return const version of end iterator
  */
  const_iterator end () const
  {
    return data () + _size;
  }
  /**
  * @return cbegin iterator
  */
  const_iterator cbegin () const
  {
    return data ();
  }

  /**
  * @return cend iterator
  */
  const_iterator cend () const
  {
    return data () + _size;
  }
  /**
   * @return rbegin iterator
   */
  reverse_iterator rbegin ()
  {
    return reverse_iterator (end ());
  }
  /**
   * @return rend iterator
   */
  reverse_iterator rend ()
  {
    return reverse_iterator (begin ());
  }
  /**
 * @return const rbegin iterator
 */
  const_reverse_iterator rbegin () const
  {
    return const_reverse_iterator (end ());
  }

  /**
 * @return const rend iterator
 */
  const_reverse_iterator rend () const
  {
    return const_reverse_iterator (begin ());
  }

  /**
 * @return crbegin iterator
 */
  const_reverse_iterator crbegin () const
  {
    return const_reverse_iterator (cend ());
  }

  /**
 * @return crend iterator
 */
  const_reverse_iterator crend () const
  {
    return const_reverse_iterator (cbegin ());
  }

 private:
  size_t _cur_capacity;
  size_t _size;
  T *_dynamic_data;
  T _static_data[StaticCapacity];

  /**
   * calculate the capacity according to the numbers of elements we insert
   * to the vector.
   * @param k number of elements to insert
   * @return the updated capacity
   */
  size_t calculate_capacity (size_t k) const
  {
    if (_size + k <= _cur_capacity)
      {
        return _cur_capacity;
      }
    else
      {
        return floorf ((THREE * (float) (_size + k)) / TWO);
      }
  }

  /**
 * copies the data n steps forward, but at the same array, and insert the new
 * sequence to the empty space we created. Inits the final to be the iterator
 * to the first inserted element.
 * @param n number of items to insert
 * @param first iterator to the first element to insert
 * @param last iterator to the one before  element to insert
 * @param pos Position in the vector where the new elements are inserted.
 * @param final iterator to the first inserted element
 */
  template<typename ForwardIterator>
  void insert_copy_data_same_array (size_t n, ForwardIterator first,
                                    ForwardIterator last,
                                    const_iterator pos, iterator &final)
  {
    size_t pos_place = std::distance (cbegin (), pos);
    std::move_backward (begin () + pos_place, end (), end () + n); // move all
    // elements from pos - end n step forward.
    std::copy (first, last, begin () + pos_place); // insert new elements from
    // 'pos' place
    final = begin () + pos_place;
    _size = _size + n;
  }
  /**
   * * copies the data from the current data to the output data, and insert the
   * new sequence before the 'pos' in the ner data.
   * Inits the final to be the iterator to the first inserted element.
   * @tparam ForwardIterator the typename of first and last iterators
   * @param n number of elements to insert
   * @param first iterator to the first element to insert
   * @param last iterator to the one before  element to insert
   * @param pos Position in the vector where the new elements are inserted.
   * @param output_data the array we copy to
   * @param final  iterator to the first inserted element
   */
  template<typename ForwardIterator>
  void insert_copy_data_other_array (size_t n, ForwardIterator first,
                                     ForwardIterator last,
                                     const_iterator pos,
                                     T *output_data, iterator &final)
  {
    size_t pos_place = std::distance (cbegin (), pos); // the distance from
    // the beginning to 'pos'
    std::move (cbegin (), pos, output_data);
    std::copy (first, last, output_data + pos_place);  // copy the new elements
    // before the position
    final = output_data + pos_place;
    std::move (pos, cend (), final + n);
    _size = _size + n;
  }
};

#endif //_VL_VECTOR_H_