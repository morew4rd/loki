#ifndef LK_INCLUDE_INTERNAL_H
#define LK_INCLUDE_INTERNAL_H

#include "gb/gb.h"

// From: string.cpp

// NOTE(bill): Used for UTF-8 strings
struct String {
	u8 *  text;
	isize len;

	u8 const &operator[](isize i) const {
		GB_ASSERT_MSG(0 <= i && i < len, "[%td]", i);
		return text[i];
	}
};
struct String_Iterator {
	String const &str;
	isize  pos;
};
// NOTE(bill): used for printf style arguments
#define LIT(x) ((int)(x).len), (x).text
#if defined(GB_COMPILER_MSVC) && _MSC_VER < 1700
	#define STR_LIT(c_str) make_string(cast(u8 *)c_str, gb_size_of(c_str)-1)
#else
	#define STR_LIT(c_str) String{cast(u8 *)c_str, gb_size_of(c_str)-1}
#endif

#define str_lit(c_str) STR_LIT(c_str)

// NOTE(bill): String16 is only used for Windows due to its file directories
struct String16 {
	wchar_t *text;
	isize    len;
	wchar_t const &operator[](isize i) const {
		GB_ASSERT_MSG(0 <= i && i < len, "[%td]", i);
		return text[i];
	}
};


// From: array.cpp (array + slice)

#define ARRAY_GROW_FORMULA(x) (gb_max(((x)+1)*3 >> 1, 8))

GB_STATIC_ASSERT(ARRAY_GROW_FORMULA(0) > 0);

template <typename T>
struct Array {
	gbAllocator allocator;
	T *         data;
	isize       count;
	isize       capacity;

	T &operator[](isize index) {
		#if !defined(NO_ARRAY_BOUNDS_CHECK)
			GB_ASSERT_MSG(cast(usize)index < cast(usize)count, "Index %td is out of bounds ranges 0..<%td", index, count);
		#endif
		return data[index];
	}

	T const &operator[](isize index) const {
		#if !defined(NO_ARRAY_BOUNDS_CHECK)
			GB_ASSERT_MSG(cast(usize)index < cast(usize)count, "Index %td is out of bounds ranges 0..<%td", index, count);
		#endif
		return data[index];
	}
};

template <typename T> void array_init          (Array<T> *array, gbAllocator const &a);
template <typename T> void array_init          (Array<T> *array, gbAllocator const &a, isize count);
template <typename T> void array_init          (Array<T> *array, gbAllocator const &a, isize count, isize capacity);
template <typename T> Array<T> array_make          (gbAllocator const &a);
template <typename T> Array<T> array_make          (gbAllocator const &a, isize count);
template <typename T> Array<T> array_make          (gbAllocator const &a, isize count, isize capacity);
template <typename T> Array<T> array_make_from_ptr (T *data, isize count, isize capacity);
template <typename T> void array_free          (Array<T> *array);
template <typename T> void array_add           (Array<T> *array, T const &t);
template <typename T> T *      array_add_and_get   (Array<T> *array);
template <typename T> void array_add_elems     (Array<T> *array, T const *elems, isize elem_count);
template <typename T> static T        array_pop           (Array<T> *array);
template <typename T> void array_clear         (Array<T> *array);
template <typename T> void array_reserve       (Array<T> *array, isize capacity);
template <typename T> void array_resize        (Array<T> *array, isize count);
template <typename T> void array_set_capacity  (Array<T> *array, isize capacity);
template <typename T> Array<T> array_slice         (Array<T> const &array, isize lo, isize hi);
template <typename T> Array<T> array_clone         (gbAllocator const &a, Array<T> const &array);

template <typename T> void array_ordered_remove  (Array<T> *array, isize index);
template <typename T> void array_unordered_remove(Array<T> *array, isize index);

template <typename T> void array_copy(Array<T> *array, Array<T> const &data, isize offset);
template <typename T> void array_copy(Array<T> *array, Array<T> const &data, isize offset, isize count);

template <typename T> T *array_end_ptr(Array<T> *array);


template <typename T>
void array_sort(Array<T> &array, gbCompareProc compare_proc) {
	gb_sort_array(array.data, array.count, compare_proc);
}



template <typename T>
struct Slice {
	T *data;
	isize count;

	gb_inline T &operator[](isize index) {
		#if !defined(NO_ARRAY_BOUNDS_CHECK)
			GB_ASSERT_MSG(cast(usize)index < cast(usize)count, "Index %td is out of bounds ranges 0..<%td", index, count);
		#endif
		return data[index];
	}

	gb_inline T const &operator[](isize index) const {
		#if !defined(NO_ARRAY_BOUNDS_CHECK)
			GB_ASSERT_MSG(cast(usize)index < cast(usize)count, "Index %td is out of bounds ranges 0..<%td", index, count);
		#endif
		return data[index];
	}
};

template <typename T> static Slice<T> slice_from_array(Array<T> const &a);


// From: common.cpp and common_memory.cpp

static gbAllocator heap_allocator(void);

#define for_array_off(index_, off_, array_) for (isize index_ = off_; index_ < (array_).count; index_++)
#define for_array(index_, array_) for_array_off(index_, 0, array_)

#endif // LK_INCLUDE_INTERNAL_H
