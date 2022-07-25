#ifndef SINGLYLINKEDLIST_H_
#define SINGLYLINKEDLIST_H_

#include <iostream>
#include <initializer_list>
#include <utility>
#include <memory>

namespace lan {

template <typename T>
struct Node {
	using _NodePointer = Node<T>*;

	Node() : Next(nullptr), Val(T()) {}

	template <typename... Args>
	Node(Args&&... args)
		: Next(nullptr), Val(std::forward<Args>(args)...) {}

	template <typename... Args>
	Node(_NodePointer p_Next, Args&&... args)
		: Next(p_Next), Val(std::forward<Args>(args)...) {}

	Node(const Node&)            = delete;
	Node& operator=(const Node&) = delete;

	~Node() {
		Next = nullptr;
		// std::cout << "Destroy Node!\n";
	}

	_NodePointer Next; // successor node
	T Val; // the stored value
};

template <class Iter, class = void>
constexpr bool isiterator = false;
	
template <class Iter>
constexpr bool isiterator<
	Iter, std::void_t<typename std::iterator_traits<Iter>::iterator_category>> = true;

template <class MyList>
class Listconst_iterator {
public:
	using _NodePointer      = typename MyList::_NodePointer;

	using iterator_category = std::forward_iterator_tag;
	using value_type        = typename MyList::value_type;
	using difference_type   = typename MyList::difference_type;
	using pointer           = typename MyList::const_pointer;
	using reference         = const value_type&;

public:
	Listconst_iterator() : m_Ptr(nullptr) {}
	Listconst_iterator(_NodePointer ptr) : m_Ptr(ptr) {}
	~Listconst_iterator() { m_Ptr = nullptr; }

	reference operator*() const {
		return m_Ptr->Val;
	}

	pointer operator->() const {
		return std::pointer_traits<pointer>::pointer_to(**this);
	}

	Listconst_iterator& operator++() {
		m_Ptr = m_Ptr->Next;
		return *this;
	}

	Listconst_iterator operator++(int) {
		Listconst_iterator Temp(m_Ptr);
		m_Ptr = m_Ptr->Next;
		return Temp;
	}

	bool operator==(const Listconst_iterator& Right) const {
		return m_Ptr == Right.m_Ptr;
	}

	bool operator!=(const Listconst_iterator& Right) const {
		return !(*this == Right);
	}

	_NodePointer m_Ptr;
};

template <class MyList>
class Listiterator : public Listconst_iterator<MyList> {
public:
	using _MyBase           = Listconst_iterator<MyList>;
	using _NodePointer      = typename MyList::_NodePointer;

	using iterator_category = std::forward_iterator_tag;
	using value_type        = typename MyList::value_type;
	using difference_type   = typename MyList::difference_type;
	using pointer           = typename MyList::pointer;
	using reference         = value_type&;

public:

	Listiterator(_NodePointer p_Ptr) : _MyBase(p_Ptr) {}

	reference operator*() const {
		return const_cast<reference>(_MyBase::operator*());
	}

	pointer operator->() const {
		return std::pointer_traits<pointer>::pointer_to(**this);
	}

	Listiterator& operator++() {
		_MyBase::operator++();
		return *this;
	}

	Listiterator operator++(int) {
		Listiterator Temp(*this);
		_MyBase::operator++();
		return Temp;
	}
};

template <typename T>
class SinglyLinkedList {
public:
	using _NodePointer   = typename Node<T>::_NodePointer;
	using _Node          = typename std::pointer_traits<_NodePointer>::element_type;

public:
	using value_type      = T;
	using reference       = value_type&;
	using const_reference = const value_type&;
	using pointer         = value_type*;
	using const_pointer   = const value_type*;
	using iterator        = Listiterator<SinglyLinkedList<value_type>>;
	using const_iterator  = Listconst_iterator<SinglyLinkedList<value_type>>;
	using difference_type = std::ptrdiff_t;
	using size_type       = std::size_t;

public:
	/////////////////
	// Constructor //
	/////////////////

	SinglyLinkedList()
		: m_Head(nullptr) {}

	SinglyLinkedList(size_type p_Count)
		: m_Head(nullptr) {
		if (p_Count == 0)
			return;

		m_Head = new _Node();
		_NodePointer curr = m_Head;
		while (--p_Count > 0) {
			const _NodePointer temp = new _Node();
			curr->Next = temp;
			curr = curr->Next;
		}
	}

	SinglyLinkedList(size_type p_Count, const value_type& Val)
		: m_Head(nullptr)  {
		if (p_Count == 0)
			return;

		m_Head = new _Node(Val);
		_NodePointer curr = m_Head;
		while (--p_Count > 0) {
			const _NodePointer temp = new _Node(Val);
			curr->Next = temp;
			curr = curr->Next;
		}
	}

	template <class Inputiterator, std::enable_if_t<isiterator<Inputiterator>, int> = 0>
	SinglyLinkedList(Inputiterator p_First, Inputiterator p_Last)
		: m_Head(nullptr) {
		m_Head = new _Node(*p_First);
		_NodePointer Prev = m_Head;
		++p_First;
		for (; p_First != p_Last; ++p_First) {
			_NodePointer NewNode = new _Node(*p_First);
			Prev->Next = NewNode;
			Prev = NewNode;
		}
	}

	SinglyLinkedList(const SinglyLinkedList& p_Other) 
		: m_Head(nullptr) {
		if (p_Other.empty())
			return;

		_NodePointer Copy = p_Other.m_Head;
		m_Head = new _Node(Copy->Val);
		_NodePointer Prev = m_Head;
		Copy = Copy->Next;
		while (Copy) {
			_NodePointer NewNode = new _Node(Copy->Val);
			Prev->Next = NewNode;
			Prev = NewNode;
			Copy = Copy->Next;
		}
	}

	SinglyLinkedList(SinglyLinkedList&& p_Other)
		: m_Head(p_Other.m_Head) {
		p_Other.m_Head = nullptr;
	}

	SinglyLinkedList(std::initializer_list<value_type> p_Ilist)
	: m_Head(nullptr) {
		insertAfter(before_begin(), p_Ilist.begin(), p_Ilist.end());
	}



	////////////////
	// Destructor //
	////////////////

	~SinglyLinkedList() {
		clear();
	}



	///////////////
	// Operator= //
	///////////////

	SinglyLinkedList& operator=(const SinglyLinkedList& p_Other) {
		clear();
		if (p_Other.empty())
			return *this;

		_NodePointer Copy = p_Other.m_Head;
		m_Head = new _Node(Copy->Val);
		_NodePointer Prev = m_Head;
		Copy = Copy->Next;
		while (Copy) {
			_NodePointer Curr = new _Node(Copy->Val);
			Prev->Next = Curr;
			Prev = Curr;
			Copy = Copy->Next;
		}
		return *this;
	}

	SinglyLinkedList& operator=(SinglyLinkedList&& p_Other) {
		if (m_Head != p_Other.m_Head) {
			clear();
			m_Head = p_Other.m_Head;
			p_Other.m_Head = nullptr;
		}
		return *this;
	}

	SinglyLinkedList& operator=(std::initializer_list<value_type> p_Ilist) {
		assign(p_Ilist.begin(), p_Ilist.end());
		return *this;
	}



	///////////////
	// Iterators //
	///////////////

private:
	_NodePointer _BeforeHead() const {
		return std::pointer_traits<_NodePointer>::pointer_to(
			reinterpret_cast<_Node&>(const_cast<_NodePointer&>(m_Head)));
	}

public:
	iterator before_begin() {
		return iterator(_BeforeHead());
	}

	const_iterator before_begin() const {
		return const_iterator(_BeforeHead());
	}

	iterator begin() {
		return iterator(m_Head);
	}

	const_iterator begin() const {
		return const_iterator(m_Head);
	}

	iterator end() {
		return iterator(nullptr);
	}

	const_iterator end() const {
		return const_iterator(nullptr);
	}

	const_iterator cbefore_begin() const {
		return before_begin();
	}

	const_iterator cbegin() const {
		return begin();
	}

	const_iterator cend() const {
		return end();
	}



	//////////////
	// Capacity //
	//////////////

	bool empty() const {
		return m_Head == nullptr;
	}
	//bool max_size();



	////////////////////
	// Element Access //
	////////////////////

	reference front() {
		return m_Head->Val;
	}
	const_reference front() const {
		return m_Head->Val;
	}



	///////////////
	// Modifiers //
	///////////////

	template <class Inputiterator, std::enable_if_t<isiterator<Inputiterator>, int> = 0>
	void assign(Inputiterator p_First, Inputiterator p_Last) {
		clear();
		insert_after(before_begin(), p_First, p_Last);
	}

	void assign(size_type p_Count, const value_type& p_Val) {
		clear();
		insert_after(before_begin(), p_Count, p_Val);
	}

	void assign(std::initializer_list<value_type> p_Ilist) {
		assign(p_Ilist.begin(), p_Ilist.end());
	}

private:
	template <typename... Args>
	void _InsertAfter(_NodePointer p_Node, Args&&... p_Args) {
		_NodePointer NewNode = new _Node(p_Node->Next, std::forward<Args>(p_Args)...);
		p_Node->Next = NewNode;
	}

	void _EraseAfter(_NodePointer p_Node) {
		_NodePointer Subject = p_Node->Next;
		p_Node->Next = Subject->Next;
		delete Subject;
	}

public:
	template <typename... Args>
	void emplace_front(Args&&... p_Args) {
		_InsertAfter(_BeforeHead(), std::forward<Args>(p_Args)...);
	}

	void push_front(const value_type& p_Val) {
		_InsertAfter(_BeforeHead(), p_Val);
	}

	void push_front(value_type&& p_Val) {
		_InsertAfter(_BeforeHead(), std::move(p_Val));
	}

	void pop_front() {
		_EraseAfter(_BeforeHead());
	}

	template <typename... Args>
	iterator emplace_after(const_iterator p_Where, Args&&... p_Args) {
		_InsertAfter(p_Where.m_Ptr, std::forward<Args>(p_Args)...);
		return iterator(p_Where.m_Ptr->Next);
	}

	iterator insert_after(const_iterator p_Where, const value_type& p_Val) {
		_InsertAfter(p_Where.m_Ptr, p_Val);
		return iterator(p_Where.m_Ptr->Next);
	}

	iterator insert_after(const_iterator p_Where, value_type&& p_Val) {
		return emplace_after(p_Where, p_Val);
	}

	iterator insert_after(const_iterator p_Where, size_type p_Count, const value_type& p_Val) {
		_NodePointer After = p_Where.m_Ptr->Next;
		while (p_Count != 0) {
			_NodePointer NewNode = new _Node(After, p_Val);
			After = NewNode;
			--p_Count;
		}
		p_Where.m_Ptr->Next = After;
		return iterator(After);
	}

	template <class Inputiterator, std::enable_if_t<isiterator<Inputiterator>, int> = 0>
	iterator insert_after(const_iterator p_Where, Inputiterator p_First, Inputiterator p_Last) {
		for (; p_First != p_Last; ++p_First) {
			p_Where = insert_after(p_Where, *p_First);
		}
		return iterator(p_Where.m_Ptr);
	}

	iterator insert_after(const_iterator p_Where, std::initializer_list<value_type> p_Ilist) {
		return insert_after(p_Where, p_Ilist.begin(), p_Ilist.end());
	}

	iterator erase_after(const_iterator p_Where) {
		_EraseAfter(p_Where.m_Ptr);
		return iterator(p_Where.m_Ptr->Next);
	}

	iterator erase_after(const_iterator p_First, const_iterator p_Last) {
		const_iterator Curr(p_First.m_Ptr->Next);
		while (Curr != p_Last) {
			_NodePointer Subject = Curr.m_Ptr;
			++Curr;
			delete Subject;
		}
		p_First.m_Ptr->Next = p_Last.m_Ptr;
		return iterator(p_Last.m_Ptr);
	}

	void swap(SinglyLinkedList& p_Other) {
		_NodePointer Temp = m_Head;
		m_Head = p_Other.m_Head;
		p_Other.m_Head = Temp;
	}

private:
	template <typename... Args>
	void _Resize(size_type p_NewSize, Args&&... p_Args) {
		_NodePointer Prev = _BeforeHead();
		for (;;) {
			_NodePointer Curr = Prev->Next;
			if (!Curr) {
				// list too short, expand
				while (p_NewSize > 0) {
					_NodePointer Temp = new _Node(p_Args...);
					Prev->Next = Temp;
					Prev = Temp;
					--p_NewSize;
				}
				return;
			}

			if (p_NewSize == 0) {
				// list too long, shrink
				Prev->Next = nullptr;
				while (Curr) {
					_NodePointer Temp = Curr;
					Curr = Curr->Next;
					delete Temp;
				}
				return;
			}

			Prev = Curr;
			--p_NewSize;
		}
	}

public:
	void resize(size_type p_NewSize) {
		_Resize(p_NewSize);
	}

	void resize(size_type p_NewSize, const value_type& p_Val) {
		_Resize(p_NewSize, p_Val);
	}

	void clear() {
		while (m_Head) {
			_NodePointer Subject = m_Head;
			m_Head = m_Head->Next;
			delete Subject;
		}
	}


	////////////////
	// Operations //
	////////////////
private:
	void _SpliceAfter(_NodePointer p_Where, _NodePointer p_Prev) {
		if (p_Where != p_Prev) {
			const _NodePointer First = p_Prev->Next;
			if (p_Where != First) {
				p_Prev->Next = First->Next;
				First->Next = p_Where->Next;
				p_Where->Next = First;
			}
		}
	}

	void _SpliceAfter(const_iterator p_Where, const_iterator p_First, const_iterator p_Last) {
		if (p_First == p_Last)
			return;

		const_iterator After = p_First;
		++After;
		if (After == p_Last)
			return;

		const_iterator Tail = p_First;
		do {
			Tail = After;
			++After;
		} while (After != p_Last);

		Tail.m_Ptr->Next = p_Where.m_Ptr->Next;
		p_Where.m_Ptr->Next = p_First.m_Ptr->Next;
		p_First.m_Ptr->Next = p_Last.m_Ptr;
	}

public:
	void spliceAfter(const_iterator p_Where, SinglyLinkedList& p_Other) {
		if (this != std::addressof(p_Other))
			_SpliceAfter(p_Where, p_Other.before_begin(), p_Other.end());
	}

	void spliceAfter(const_iterator p_Where, SinglyLinkedList&& p_Other) {
		spliceAfter(p_Where, p_Other);
	}

	void spliceAfter(const_iterator p_Where, SinglyLinkedList& p_Other, const_iterator p_First) {
		(void)p_Other;
		_SpliceAfter(p_Where.m_Ptr, p_First.m_Ptr);
	}

	void spliceAfter(const_iterator p_Where, SinglyLinkedList&& p_Other, const_iterator p_Target) {
		spliceAfter(p_Where, p_Other, p_Target);
	}

	void spliceAfter(const_iterator p_Where, SinglyLinkedList& p_Other,
					 const_iterator p_First, const_iterator p_Last) {
		(void)p_Other;
		_SpliceAfter(p_Where, p_First, p_Last);
	}

	void spliceAfter(const_iterator p_Where, SinglyLinkedList&& p_Other,
					 const_iterator p_First, const_iterator p_Last) {
		spliceAfter(p_Where, p_Other, p_First, p_Last);
	}

	void remove(const value_type& p_Val) {
		removeIf([&](const value_type& p_Each) { return p_Each = p_Val; });
	}

	template <class Predicate>
	void removeIf(Predicate p_Pred) {
		_NodePointer Curr = m_Head;
		_NodePointer Prev = _BeforeHead();
		while (Curr) {
			if (p_Pred(Curr->Val)) {
				Prev->Next = Curr->Next;
				delete Curr;
				Curr = Prev->Next;
			} else {
				Prev = Curr;
				Curr = Curr->Next;
			}
		}
	}

	void unique() {
		unique([](const value_type& p_First, const value_type& p_Second) { return p_First == p_Second; });
	}

	template <class Predicate>
	void unique(Predicate p_Pred) {
		_NodePointer Curr = m_Head;
		_NodePointer Prev = _BeforeHead();
		while (Curr) {
			if (p_Pred(Prev->Val, Curr->Val)) {
				Prev->Next = Curr->Next;
				delete Curr;
				Curr = Prev->Next;
			} else {
				Prev = Curr;
				Curr = Curr->Next;
			}
		}
	}

	//void merge();
	//void sort();
	//void reverse();

private:
	_NodePointer m_Head;
};

} // namespace lan

#endif // !SINGLYLINKEDLIST_H_