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
constexpr bool isIterator = false;
	
template <class Iter>
constexpr bool isIterator<
	Iter, std::void_t<typename std::iterator_traits<Iter>::iterator_category>> = true;

template <class MyList>
class ListConstIterator {
public:
	using _NodePointer      = typename MyList::_NodePointer;

	using iterator_category = std::forward_iterator_tag;
	using value_type        = typename MyList::ValueType;
	using difference_type   = typename MyList::DifferenceType;
	using pointer           = typename MyList::ConstPointer;
	using reference         = const value_type&;

public:
	ListConstIterator() : m_Ptr(nullptr) {}
	ListConstIterator(_NodePointer ptr) : m_Ptr(ptr) {}
	~ListConstIterator() { m_Ptr = nullptr; }

	reference operator*() const {
		return m_Ptr->Val;
	}

	pointer operator->() const {
		return std::pointer_traits<pointer>::pointer_to(**this);
	}

	ListConstIterator& operator++() {
		m_Ptr = m_Ptr->Next;
		return *this;
	}

	ListConstIterator operator++(int) {
		ListConstIterator Temp(m_Ptr);
		m_Ptr = m_Ptr->Next;
		return Temp;
	}

	bool operator==(const ListConstIterator& Right) const {
		return m_Ptr == Right.m_Ptr;
	}

	bool operator!=(const ListConstIterator& Right) const {
		return !(*this == Right);
	}

	_NodePointer m_Ptr;
};

template <class MyList>
class ListIterator : public ListConstIterator<MyList> {
public:
	using _MyBase           = ListConstIterator<MyList>;
	using _NodePointer      = typename MyList::_NodePointer;

	using iterator_category = std::forward_iterator_tag;
	using value_type        = typename MyList::ValueType;
	using difference_type   = typename MyList::DifferenceType;
	using pointer           = typename MyList::Pointer;
	using reference         = value_type&;

public:

	ListIterator(_NodePointer p_Ptr) : _MyBase(p_Ptr) {}

	reference operator*() const {
		return const_cast<reference>(_MyBase::operator*());
	}

	pointer operator->() const {
		return std::pointer_traits<pointer>::pointer_to(**this);
	}

	ListIterator& operator++() {
		_MyBase::operator++();
		return *this;
	}

	ListIterator operator++(int) {
		ListIterator Temp(*this);
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
	using ValueType      = T;
	using Reference      = ValueType&;
	using ConstReference = const ValueType&;
	using Pointer        = ValueType*;
	using ConstPointer   = const ValueType*;
	using Iterator       = ListIterator<SinglyLinkedList<ValueType>>;
	using ConstIterator  = ListConstIterator<SinglyLinkedList<ValueType>>;
	using DifferenceType = std::ptrdiff_t;
	using SizeType       = std::size_t;

public:
	/////////////////
	// Constructor //
	/////////////////

	SinglyLinkedList()
		: m_Head(nullptr) {}

	SinglyLinkedList(SizeType p_Count)
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

	SinglyLinkedList(SizeType p_Count, const ValueType& Val)
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

	template <class InputIterator, std::enable_if_t<isIterator<InputIterator>, int> = 0>
	SinglyLinkedList(InputIterator p_First, InputIterator p_Last)
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

	SinglyLinkedList(std::initializer_list<ValueType> p_Ilist)
	: m_Head(nullptr) {
		insertAfter(beforeBegin(), p_Ilist.begin(), p_Ilist.end());
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

	SinglyLinkedList& operator=(std::initializer_list<ValueType> p_Ilist) {
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
	Iterator beforeBegin() {
		return Iterator(_BeforeHead());
	}

	ConstIterator beforeBegin() const {
		return ConstIterator(_BeforeHead());
	}

	Iterator begin() {
		return Iterator(m_Head);
	}

	ConstIterator begin() const {
		return ConstIterator(m_Head);
	}

	Iterator end() {
		return Iterator(nullptr);
	}

	ConstIterator end() const {
		return ConstIterator(nullptr);
	}

	ConstIterator cBeforeBegin() const {
		return beforeBegin();
	}

	ConstIterator cBegin() const {
		return begin();
	}

	ConstIterator cEnd() const {
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

	Reference front() {
		return m_Head->Val;
	}
	ConstReference front() const {
		return m_Head->Val;
	}



	///////////////
	// Modifiers //
	///////////////

	template <class InputIterator, std::enable_if_t<isIterator<InputIterator>, int> = 0>
	void assign(InputIterator p_First, InputIterator p_Last) {
		clear();
		insertAfter(beforeBegin(), p_First, p_Last);
	}

	void assign(SizeType p_Count, const ValueType& p_Val) {
		clear();
		insertAfter(beforeBegin(), p_Count, p_Val);
	}

	void assign(std::initializer_list<ValueType> p_Ilist) {
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
	void emplaceFront(Args&&... p_Args) {
		_InsertAfter(_BeforeHead(), std::forward<Args>(p_Args)...);
	}

	void pushFront(const ValueType& p_Val) {
		_InsertAfter(_BeforeHead(), p_Val);
	}

	void pushFront(ValueType&& p_Val) {
		_InsertAfter(_BeforeHead(), std::move(p_Val));
	}

	void popFront() {
		_EraseAfter(_BeforeHead());
	}

	template <typename... Args>
	Iterator emplaceAfter(ConstIterator p_Where, Args&&... p_Args) {
		_InsertAfter(p_Where.m_Ptr, std::forward<Args>(p_Args)...);
		return Iterator(p_Where.m_Ptr->Next);
	}

	Iterator insertAfter(ConstIterator p_Where, const ValueType& p_Val) {
		_InsertAfter(p_Where.m_Ptr, p_Val);
		return Iterator(p_Where.m_Ptr->Next);
	}

	Iterator insertAfter(ConstIterator p_Where, ValueType&& p_Val) {
		return emplaceAfter(p_Where, p_Val);
	}

	Iterator insertAfter(ConstIterator p_Where, SizeType p_Count, const ValueType& p_Val) {
		_NodePointer After = p_Where.m_Ptr->Next;
		while (p_Count != 0) {
			_NodePointer NewNode = new _Node(After, p_Val);
			After = NewNode;
			--p_Count;
		}
		p_Where.m_Ptr->Next = After;
		return Iterator(After);
	}

	template <class InputIterator, std::enable_if_t<isIterator<InputIterator>, int> = 0>
	Iterator insertAfter(ConstIterator p_Where, InputIterator p_First, InputIterator p_Last) {
		for (; p_First != p_Last; ++p_First) {
			p_Where = insertAfter(p_Where, *p_First);
		}
		return Iterator(p_Where.m_Ptr);
	}

	Iterator insertAfter(ConstIterator p_Where, std::initializer_list<ValueType> p_Ilist) {
		return insertAfter(p_Where, p_Ilist.begin(), p_Ilist.end());
	}

	Iterator eraseAfter(ConstIterator p_Where) {
		_EraseAfter(p_Where.m_Ptr);
		return Iterator(p_Where.m_Ptr->Next);
	}

	Iterator eraseAfter(ConstIterator p_First, ConstIterator p_Last) {
		ConstIterator Curr(p_First.m_Ptr->Next);
		while (Curr != p_Last) {
			_NodePointer Subject = Curr.m_Ptr;
			++Curr;
			delete Subject;
		}
		p_First.m_Ptr->Next = p_Last.m_Ptr;
		return Iterator(p_Last.m_Ptr);
	}

	void swap(SinglyLinkedList& p_Other) {
		_NodePointer Temp = m_Head;
		m_Head = p_Other.m_Head;
		p_Other.m_Head = Temp;
	}
private:
	template <typename... Args>
	void _Resize(SizeType p_NewSize, Args&&... p_Args) {
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
	void resize(SizeType p_NewSize) {
		_Resize(p_NewSize);
	}

	void resize(SizeType p_NewSize, const ValueType& p_Val) {
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

	void _SpliceAfter(ConstIterator p_Where, ConstIterator p_First, ConstIterator p_Last) {
		if (p_First == p_Last)
			return;

		ConstIterator After = p_First;
		++After;
		if (After == p_Last)
			return;

		ConstIterator Tail = p_First;
		do {
			Tail = After;
			++After;
		} while (After != p_Last);

		Tail.m_Ptr->Next = p_Where.m_Ptr->Next;
		p_Where.m_Ptr->Next = p_First.m_Ptr->Next;
		p_First.m_Ptr->Next = p_Last.m_Ptr;
	}

public:
	void spliceAfter(ConstIterator p_Where, SinglyLinkedList& p_Other) {
		if (this != std::addressof(p_Other))
			_SpliceAfter(p_Where, p_Other.beforeBegin(), p_Other.end());
	}

	void spliceAfter(ConstIterator p_Where, SinglyLinkedList&& p_Other) {
		spliceAfter(p_Where, p_Other);
	}

	void spliceAfter(ConstIterator p_Where, SinglyLinkedList& p_Other, ConstIterator p_First) {
		(void)p_Other;
		_SpliceAfter(p_Where.m_Ptr, p_First.m_Ptr);
	}

	void spliceAfter(ConstIterator p_Where, SinglyLinkedList&& p_Other, ConstIterator p_Target) {
		spliceAfter(p_Where, p_Other, p_Target);
	}

	void spliceAfter(ConstIterator p_Where, SinglyLinkedList& p_Other,
					 ConstIterator p_First, ConstIterator p_Last) {
		(void)p_Other;
		_SpliceAfter(p_Where, p_First, p_Last);
	}

	void spliceAfter(ConstIterator p_Where, SinglyLinkedList&& p_Other,
					 ConstIterator p_First, ConstIterator p_Last) {
		spliceAfter(p_Where, p_Other, p_First, p_Last);
	}

	void remove(const ValueType& p_Val) {
		removeIf([&](const ValueType& p_Each) { return p_Each = p_Val; });
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
		unique([](const ValueType& p_First, const ValueType& p_Second) { return p_First == p_Second; });
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