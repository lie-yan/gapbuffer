#pragma once

#include <memory>
#include <iterator>
#include <cstddef>
#include <gsl/gsl>

namespace dr {

/// \brief round `s` up to the nearest multiple of n
/// \tparam T integral type
/// \param s
/// \param n
/// \return the least multiple of `n` at least `s`
template<typename T>
T round_up(T s, unsigned int n) { return ((s + n - 1) / n) * n; }

template<typename charT, typename Alloc = std::allocator<charT>>
struct gap_buffer {
  using value_type      = charT;
  using difference_type = ptrdiff_t;
  using size_type       = size_t;
  using reference       = value_type&;
  using const_reference = const value_type&;
  using pointer         = value_type*;
  using const_pointer   = const value_type*;

  static constexpr float incremental_factor = 0.2;
  static constexpr size_type default_size = 8;
  static constexpr size_type alignment = 8;

  struct const_iterator;
  struct const_reverse_iterator;

  struct iterator {
    using self_type         = iterator;
    using container_type    = gap_buffer;

    using value_type        = container_type::value_type;
    using difference_type   = container_type::difference_type;
    using reference         = container_type::reference;
    using pointer           = container_type::pointer;
    using iterator_category = std::random_access_iterator_tag;

    explicit iterator(gap_buffer* container = nullptr,
                      difference_type offset = 0)
        : container(container),
          offset(offset) { }

    operator const_iterator() const
    {
      return const_iterator(container, offset);
    }

    reference operator [](difference_type i) const
    {
      return container->operator [](offset + i);
    }

    reference operator *() const
    {
      return container->operator [](offset);
    }

    pointer operator ->() const
    {
      return &(container->operator [](offset));
    }

    self_type& operator ++()
    {
      offset++;
      return *this;
    }

    self_type operator ++(int)
    {
      self_type retval = *this;
      this->operator ++();
      return retval;
    }

    self_type& operator --()
    {
      offset--;
      return *this;
    }

    self_type operator --(int)
    {
      self_type retval = *this;
      this->operator --();
      return retval;
    }

    bool operator ==(const self_type& other) const
    {
      return container == other.container && offset == other.offset;
    }

    bool operator !=(const self_type& other) const
    {
      return !(*this == other);
    }

    bool operator <(const self_type& other) const
    {
      Expects(container == other.container);
      return offset < other.offset;
    }

    bool operator >(const self_type& other) const
    {
      return other < *this;
    }

    bool operator <=(const self_type& other) const
    {
      return !(other < *this);
    }

    bool operator >=(const self_type& other) const
    {
      return !(*this < other);
    }

    self_type& operator +=(difference_type n)
    {
      offset += n;
      return *this;
    }

    friend
    self_type operator +(self_type it, difference_type n)
    {
      it.offset += n;
      return it;
    }

    friend
    self_type operator +(difference_type n, self_type it)
    {
      it.offset += n;
      return it;
    }

    self_type& operator -=(difference_type n)
    {
      offset -= n;
      return *this;
    }

    self_type operator -(difference_type n) const
    {
      return self_type(container, offset - n);
    }

    difference_type operator -(const self_type& other) const
    {
      Expects(container == other.container);
      return offset - other.offset;
    }

    friend class gap_buffer;

  private:
    gap_buffer* container;
    difference_type offset;
  };

  struct const_iterator {
    using self_type         = const_iterator;
    using container_type    = gap_buffer;

    using value_type        = container_type::value_type;
    using difference_type   = container_type::difference_type;
    using reference         = container_type::const_reference;
    using pointer           = container_type::const_pointer;
    using iterator_category = std::random_access_iterator_tag;

    explicit const_iterator(const gap_buffer* container = nullptr, difference_type offset = 0)
        : container(container),
          offset(offset) { }

    reference operator [](difference_type i) const
    {
      return container->operator [](offset + i);
    }

    reference operator *() const
    {
      return container->operator [](offset);
    }

    pointer operator ->() const
    {
      return &(container->operator [](offset));
    }

    self_type& operator ++()
    {
      offset++;
      return *this;
    }

    self_type operator ++(int)
    {
      self_type retval = *this;
      this->operator ++();
      return retval;
    }

    self_type& operator --()
    {
      offset--;
      return *this;
    }

    self_type operator --(int)
    {
      self_type retval = *this;
      this->operator --();
      return retval;
    }

    bool operator ==(const self_type& other) const
    {
      return container == other.container && offset == other.offset;
    }

    bool operator !=(const self_type& other) const
    {
      return !(*this == other);
    }

    bool operator <(const self_type& other) const
    {
      Expects(container == other.container);
      return offset < other.offset;
    }

    bool operator >(const self_type& other) const
    {
      return other < *this;
    }

    bool operator <=(const self_type& other) const
    {
      return !(other < *this);
    }

    bool operator >=(const self_type& other) const
    {
      return !(*this < other);
    }

    self_type& operator +=(difference_type n)
    {
      offset += n;
      return *this;
    }

    friend
    self_type operator +(self_type it, difference_type n)
    {
      it.offset += n;
      return it;
    }

    friend
    self_type operator +(difference_type n, self_type it)
    {
      it.offset += n;
      return it;
    }

    self_type& operator -=(difference_type n)
    {
      offset -= n;
      return *this;
    }

    self_type operator -(difference_type n) const
    {
      return self_type(container, offset - n);
    }

    difference_type operator -(const self_type& other) const
    {
      Expects(container == other.container);
      return offset - other.offset;
    }

    friend class gap_buffer;

  private:
    const gap_buffer* container;
    difference_type offset;
  };

  struct reverse_iterator {
    using self_type         = reverse_iterator;
    using container_type    = gap_buffer;

    using value_type        = container_type::value_type;
    using difference_type   = container_type::difference_type;
    using reference         = container_type::reference;
    using pointer           = container_type::pointer;
    using iterator_category = std::random_access_iterator_tag;

    explicit reverse_iterator(gap_buffer* container = nullptr, difference_type offset = 0)
        : container(container),
          offset(offset) { }

    operator const_reverse_iterator() const
    {
      return const_reverse_iterator(container, offset);
    }

    reference operator [](difference_type i) const
    {
      return container->operator [](offset + i);
    }

    reference operator *() const
    {
      return container->operator [](offset);
    }

    pointer operator ->() const
    {
      return &(container->operator [](offset));
    }

    self_type& operator ++()
    {
      offset--;
      return *this;
    }

    self_type operator ++(int)
    {
      self_type retval = *this;
      this->operator ++();
      return retval;
    }

    self_type& operator --()
    {
      offset++;
      return *this;
    }

    self_type operator --(int)
    {
      self_type retval = *this;
      this->operator --();
      return retval;
    }

    bool operator ==(const self_type& other) const
    {
      return container == other.container && offset == other.offset;
    }

    bool operator !=(const self_type& other) const
    {
      return !(*this == other);
    }

    bool operator <(const self_type& other) const
    {
      Expects(container == other.container);
      return offset > other.offset;
    }

    bool operator >(const self_type& other) const
    {
      return other < *this;
    }

    bool operator <=(const self_type& other) const
    {
      return !(other < *this);
    }

    bool operator >=(const self_type& other) const
    {
      return !(*this < other);
    }

    self_type& operator +=(difference_type n)
    {
      offset -= n;
      return *this;
    }

    friend
    self_type operator +(self_type it, difference_type n)
    {
      it.offset -= n;
      return it;
    }

    friend
    self_type operator +(difference_type n, self_type it)
    {
      it.offset -= n;
      return it;
    }

    self_type& operator -=(difference_type n)
    {
      offset += n;
      return *this;
    }

    self_type operator -(difference_type n) const
    {
      return self_type(container, offset + n);
    }

    difference_type operator -(const self_type& other) const
    {
      Expects(container == other.container);
      return other.offset - offset;
    }

    friend class gap_buffer;

  private:
    gap_buffer* container;
    difference_type offset;
  };

  struct const_reverse_iterator {

    using self_type         = const_reverse_iterator;
    using container_type    = gap_buffer;

    using value_type        = container_type::value_type;
    using difference_type   = container_type::difference_type;
    using reference         = container_type::const_reference;
    using pointer           = container_type::const_pointer;
    using iterator_category = std::random_access_iterator_tag;

    explicit const_reverse_iterator(const gap_buffer* container = nullptr,
                                    difference_type offset = 0)
        : container(container),
          offset(offset) { }

    reference operator [](difference_type i) const
    {
      return container->operator [](offset + i);
    }

    reference operator *() const
    {
      return container->operator [](offset);
    }

    pointer operator ->() const
    {
      return &(container->operator [](offset));
    }

    self_type& operator ++()
    {
      offset--;
      return *this;
    }

    self_type operator ++(int)
    {
      self_type retval = *this;
      this->operator ++();
      return retval;
    }

    self_type& operator --()
    {
      offset++;
      return *this;
    }

    self_type operator --(int)
    {
      self_type retval = *this;
      this->operator --();
      return retval;
    }

    bool operator ==(const self_type& other) const
    {
      return container == other.container && offset == other.offset;
    }

    bool operator !=(const self_type& other) const
    {
      return !(*this == other);
    }

    bool operator <(const self_type& other) const
    {
      Expects(container == other.container);
      return offset > other.offset;
    }

    bool operator >(const self_type& other) const
    {
      return other < *this;
    }

    bool operator <=(const self_type& other) const
    {
      return !(other < *this);
    }

    bool operator >=(const self_type& other) const
    {
      return !(*this < other);
    }

    self_type& operator +=(difference_type n)
    {
      offset -= n;
      return *this;
    }

    friend
    self_type operator +(self_type it, difference_type n)
    {
      it.offset -= n;
      return it;
    }

    friend
    self_type operator +(difference_type n, self_type it)
    {
      it.offset -= n;
      return it;
    }

    self_type& operator -=(difference_type n)
    {
      offset += n;
      return *this;
    }

    self_type operator -(difference_type n) const
    {
      return self_type(container, offset + n);
    }

    difference_type operator -(const self_type& other) const
    {
      Expects(container == other.container);
      return other.offset - offset;
    }

    friend class gap_buffer;

  private:
    const gap_buffer* container;
    difference_type offset;
  };

public:

  explicit gap_buffer(size_type n = default_size)
  {
    start = allocate_and_construct(n);
    finish = start + n;
    gap_start = start;
    gap_size = n;
  }

  template<typename InputIt>
  gap_buffer(InputIt first, InputIt last)
  {
    difference_type n = std::distance(first, last);
    size_type len = round_up(std::max(default_size, size_type(n)), alignment);

    start = data_allocator.allocate(len);
    finish = start + len;

    try
    {
      gap_start = std::uninitialized_copy(first, last, start);
    }
    catch (...)
    {
      data_allocator.deallocate(start, finish - start);
      throw;
    }

    try
    {
      std::uninitialized_default_construct(gap_start, finish);
    }
    catch (...)
    {
      std::destroy(start, start + n);
      data_allocator.deallocate(start, finish - start);
      throw;
    }

    gap_size = len - n;
  }

  gap_buffer(const gap_buffer& rhs)
      : gap_buffer(rhs.begin(), rhs.end()) { }

  void swap(gap_buffer& rhs)
  {
    using std::swap;
    swap(start, rhs.start);
    swap(finish, rhs.finish);
    swap(gap_start, rhs.gap_start);
    swap(gap_size, rhs.gap_size);
  }

  gap_buffer& operator =(const gap_buffer& rhs)
  {
    gap_buffer temp(rhs);
    swap(temp);
    return *this;
  }

  gap_buffer(gap_buffer&& rhs)
      : gap_buffer() { swap(rhs); }

  gap_buffer& operator =(gap_buffer&& rhs)
  {
    swap(rhs);
    return *this;
  }

  ~gap_buffer()
  {
    destroy_and_deallocate(start, finish);
    start = finish = gap_start = nullptr;
    gap_size = 0;
  }

  friend
  bool operator ==(const gap_buffer& lhs, const gap_buffer& rhs)
  {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  friend
  bool operator !=(const gap_buffer& lhs, const gap_buffer& rhs)
  {
    return !(lhs == rhs);
  }

  bool operator <(const gap_buffer& rhs) const
  {
    auto[left, right] = std::mismatch(begin(), end(), rhs.begin(), rhs.end());

    if (right == rhs.end()) return false;
    else if (left == end()) return true;
    else return *left < *right;
  }

  bool operator >(const gap_buffer& rhs) const
  {
    return rhs < *this;
  }

  bool operator <=(const gap_buffer& rhs) const
  {
    return !(rhs < *this);
  }

  bool operator >=(const gap_buffer& rhs) const
  {
    return !(*this < rhs);
  }

  // ------ basis START HERE ------

  const_reference operator [](size_type pos) const
  {
    if (start + pos < gap_start) return *(start + pos);
    else return *(start + gap_size + pos);
  }

  iterator erase(const_iterator first, const_iterator last)
  {
    Expects(first.container == this && last.container == this);
    difference_type num_to_erase = std::distance(first, last);
    relocate_gap(first.offset);
    std::fill_n(gap_start + gap_size, num_to_erase, charT{});
    gap_size += num_to_erase;
    return iterator(this, first.offset);
  }

  ///
  /// \tparam InputIt
  /// \param pos
  /// \param first
  /// \param last
  /// \return iterator to the first inserted element
  template<class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last)
  {
    Expects(this == pos.container && pos <= end());

    difference_type num_to_insert = std::distance(first, last);
    if (gap_size >= num_to_insert)
    {
      relocate_gap(pos.offset);

      std::copy(first, last, gap_start);
      gap_start += num_to_insert;
      gap_size -= num_to_insert;
      return iterator(this, pos.offset);
    }
    else
    {
      size_type old_size = size();
      size_type old_capacity = capacity();
      auto default_delta = size_type(old_capacity * incremental_factor);
      size_type delta = round_up(std::max(default_delta, num_to_insert - gap_size), alignment);
      size_type new_capacity = std::max(old_capacity + delta, default_size);

      gap_buffer temp(new_capacity);

      relocate_gap(pos.offset);
      auto cursor = std::copy(start, gap_start, temp.start);
      cursor = std::copy(first, last, cursor);
      std::copy(gap_start + gap_size, finish, cursor);

      swap(temp);

      gap_start = start + old_size + num_to_insert;
      gap_size = finish - gap_start;

      return iterator(this, pos.offset);
    }
  }

  void reserve(size_type new_cap = 0)
  {
    if (capacity() >= new_cap) return;

    size_type old_size = size();
    size_type old_capacity = capacity();
    size_type new_capacity = round_up(new_cap, alignment);

    gap_buffer temp(new_capacity);
    auto cursor = std::copy(start, gap_start, temp.start);
    std::copy(gap_start + gap_size, finish, cursor);
    swap(temp);

    gap_start = start + old_size;
    gap_size = finish - gap_start;
  }

  size_type size() const { return finish - start - gap_size; }
  size_type length() const { return size(); }
  size_type capacity() const { return finish - start; }

  // ------ basis END HERE ------

  template<typename InputIt>
  gap_buffer& assign(InputIt first, InputIt last)
  {
    erase(begin(), end());
    insert(begin(), first, last);
    return *this;
  }

  reference operator [](size_type pos)
  {
    return const_cast<reference>(
        static_cast<const gap_buffer&>(*this).operator [](pos)
    );
  }

  const_reference at(size_type pos) const
  {
    if (pos >= size()) throw std::out_of_range("index out of range");
    return operator [](pos);
  }

  reference at(size_type pos)
  {
    return const_cast<reference>(
        static_cast<const gap_buffer&>(*this).at(pos)
    );
  }

  const_reference front() const { return operator [](0); }
  reference front()
  {
    return const_cast<reference>(
        static_cast<const gap_buffer&>(*this).front()
    );
  }

  const_reference back() const { return operator [](size() - 1); }
  reference back()
  {
    return const_cast<reference>(
        static_cast<const gap_buffer&>(*this).back()
    );
  }

  bool empty() const { return size() == 0; }

  void clear() { erase(begin(), end()); }

  /// Erases n characters, starting with the i-th element.
  /// \param i
  /// \param n
  void erase(size_type i, size_type n)
  {
    Expects(size() >= n);
    const_iterator first(this, i);
    erase(first, first + n);
  }

  /// Inserts the character `c` before the i-th element.
  /// \param i
  /// \param c
  void insert(size_type i, charT c)
  {
    insert(const_iterator(this, i), &c, &c + 1);
  }

  void push_back(charT c) { append(c); }
  void pop_back() { erase(size() - 1, 1); }

  template<typename InputIt>
  void append(InputIt first, InputIt last)
  {
    insert(end(), first, last);
  }

  void append(charT c)
  {
    insert(end(), &c, &c + 1);
  }

  template<typename InputIt>
  void replace(const_iterator f1, const_iterator l1, InputIt f2, InputIt l2)
  {
    auto cursor = erase(f1, l1);
    insert(cursor, f2, l2);
  }

  template<typename InputIt>
  void replace(const_iterator pos, InputIt first, InputIt last)
  {
    replace(pos, pos + 1, first, last);
  }

  gap_buffer substr(const_iterator first, const_iterator last) const
  {
    return substr_impl<gap_buffer>(first, last);
  }

  const_iterator begin() const { return const_iterator(this); }
  const_iterator cbegin() const { return begin(); }

  const_iterator end() const { return const_iterator(this, size()); }
  const_iterator cend() const { return end(); }

  iterator begin() { return iterator(this); }

  iterator end() { return iterator(this, size()); }

  const_reverse_iterator rbegin() const { return const_reverse_iterator(this, size() - 1); }
  const_reverse_iterator crbegin() const { return rbegin(); }

  const_reverse_iterator rend() const { return const_reverse_iterator(this, -1); }
  const_reverse_iterator crend() const { return rend(); }

  reverse_iterator rbegin() { return reverse_iterator(this, size() - 1); }

  reverse_iterator rend() { return reverse_iterator(this, -1); }

protected:

  template<typename U>
  U substr_impl(const_iterator first, const_iterator last) const
  {
    return U(first, last);
  }

  void relocate_gap(difference_type offset)
  {
    if (gap_start != start + offset)
    {
      if (gap_start < start + offset)
        std::move(gap_start /**/ + gap_size,
                  start + offset + gap_size,
                  gap_start);
      else
        std::move_backward(start + offset,
                           gap_start,
                           gap_start + gap_size);

      gap_start = start + offset;
    }
  }

  pointer allocate_and_construct(size_type n)
  {
    pointer result = data_allocator.allocate(n);
    std::uninitialized_default_construct_n(result, n);
    return result;
  }

  void destroy_and_deallocate(pointer start, pointer finish)
  {
    std::destroy(start, finish);
    if (start)
      data_allocator.deallocate(start, finish - start);
  }

private:
  Alloc data_allocator;

  pointer start;
  pointer finish;
  pointer gap_start;
  size_type gap_size;
};


}
