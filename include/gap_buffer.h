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

template<typename T, typename Allocator = std::allocator<T>>
struct gap_buffer {
  using value_type      = T;
  using allocator_type  = Allocator;
  using size_type       = std::size_t;
  using difference_type = ptrdiff_t;
  using reference       = value_type&;
  using const_reference = const value_type&;
  using pointer         = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer   = typename std::allocator_traits<Allocator>::const_pointer;

  struct iterator;
  struct const_iterator;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  struct iterator {
    using self_type         = iterator;
    using container_type    = gap_buffer;

    using value_type        = container_type::value_type;
    using difference_type   = container_type::difference_type;
    using reference         = container_type::reference;
    using pointer           = container_type::pointer;
    using iterator_category = std::random_access_iterator_tag;

    explicit iterator(gap_buffer* container = nullptr, difference_type offset = 0)
        : container(container), offset(offset) { }

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

private:
  static constexpr float incremental_factor = 0.2;
  static constexpr size_type default_size = 8;
  static constexpr size_type alignment = 8;

public:
  explicit gap_buffer(size_type count = default_size)
  {
    count = round_up(count, alignment);

    start = allocate_and_construct(count);
    finish = start + count;
    gap_start = start;
    gap_size = count;
  }

  gap_buffer(size_type count, const T& value)
      : gap_buffer(count)
  {
    for (size_type i = 0; i < count; ++i) (*this)[i] = value;
  }

  template<typename InputIt>
  gap_buffer(InputIt first, InputIt last)
  {
    difference_type n = std::distance(first, last);
    size_type len = round_up(std::max(default_size, size_type(n)), alignment);

    start = data_allocator.allocate(len);
    finish = start + len;

    int except_flag = 0;
    try
    {
      gap_start = std::uninitialized_copy(first, last, start);
      except_flag = 1;
      std::uninitialized_default_construct(gap_start, finish);
    }
    catch (...)
    {
      switch (except_flag)
      {
      case 1:
        std::destroy(start, start + n);
        // FALL THROUGH
      case 0:
        data_allocator.deallocate(start, finish - start);
      default:;// DO NOTHING
      }
      throw;
    }

    gap_size = len - n;
  }

  gap_buffer(const gap_buffer& rhs)
      : gap_buffer(rhs.begin(), rhs.end()) { }

  gap_buffer(gap_buffer&& rhs) noexcept
      : gap_buffer() { swap(rhs); }

  gap_buffer(std::initializer_list<T> ilist)
      : gap_buffer(ilist.begin(), ilist.end()) { }

  gap_buffer& operator =(const gap_buffer& rhs)
  {
    gap_buffer temp(rhs);
    swap(temp);
    return *this;
  }

  gap_buffer& operator =(gap_buffer&& rhs) noexcept
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

  void swap(gap_buffer& rhs)
  {
    using std::swap;
    swap(start, rhs.start);
    swap(finish, rhs.finish);
    swap(gap_start, rhs.gap_start);
    swap(gap_size, rhs.gap_size);
  }

  void assign(size_type count, const T& value) { *this = gap_buffer(count, value); }

  template<typename InputIt>
  void assign(InputIt first, InputIt last) { *this = gap_buffer(first, last); }

  void assign(std::initializer_list<T> ilist) { *this = gap_buffer(ilist); }

  allocator_type get_allocator() const { return data_allocator; }


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
    std::fill_n(gap_start + gap_size, num_to_erase, T{});
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
    if (new_cap > max_size()) throw std::length_error("new_cap should be less than max_size()");

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

  size_type size() const noexcept { return finish - start - gap_size; }
  size_type max_size() const noexcept { return size_type(-1); }
  size_type capacity() const noexcept { return finish - start; }

  // ------ basis END HERE ------

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

  const_reference front() const { return (*this)[0]; }
  reference front()
  {
    return const_cast<reference>(
        static_cast<const gap_buffer&>(*this).front()
    );
  }

  const_reference back() const { return (*this)[size() - 1]; }
  reference back()
  {
    return const_cast<reference>(
        static_cast<const gap_buffer&>(*this).back()
    );
  }

  T* data() noexcept  = delete;
  const T* data() const noexcept = delete;

  [[nodiscard]] bool empty() const noexcept { return size() == 0; }

  void shrink_to_fit() { /*DO NOTHNG*/ }

  void clear() { erase(begin(), end()); }

  iterator insert(const_iterator pos, const T& value)
  {
    return insert(pos, &value, &value + 1);
  }

  iterator insert(const_iterator pos, T&& value)
  {
    return insert(pos, std::make_move_iterator(&value), std::make_move_iterator(&value + 1));
  }

  iterator insert(const_iterator pos, size_type count, const T& value)
  {
    for (size_type i = 0; i < count; ++i)
      pos = insert(pos, value);
    return iterator(this, pos.offset);
  }

  iterator insert(const_iterator pos, std::initializer_list<T> ilist)
  {
    return insert(pos, ilist.begin(), ilist.end());
  }

  void erase(const_iterator pos) { erase(pos, pos + 1); }

  void push_back(const T& value) { insert(end(), &value, &value + 1); }
  void push_back(T&& value) { insert(end(), std::make_move_iterator(&value), std::make_move_iterator(&value + 1)); }
  void pop_back() { erase(end() - 1); }

  const_iterator begin() const noexcept { return const_iterator(this); }
  const_iterator cbegin() const noexcept { return begin(); }

  const_iterator end() const noexcept { return const_iterator(this, size()); }
  const_iterator cend() const noexcept { return end(); }

  iterator begin() noexcept { return iterator(this); }
  iterator end() noexcept { return iterator(this, size()); }

  const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
  const_reverse_iterator crbegin() const noexcept { return rbegin(); }

  const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
  const_reverse_iterator crend() const noexcept { return rend(); }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

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

  // additional
  template<typename InputIt>
  void append(InputIt first, InputIt last)
  {
    insert(end(), first, last);
  }

  void append(const T& value) { push_back(value); }

  void append(T&& value) { push_back(std::move(value)); }

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
  Allocator data_allocator;

  pointer start;
  pointer finish;
  pointer gap_start;
  size_type gap_size;
};


}
