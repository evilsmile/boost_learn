为了管理内存等资源，C++程序员通常采用RAII(resource acquisition is initialization, 资源获取即初始化)，在使用资源的类构造函数中申请资源，然后使用，最后在析构函数中释放资源。

shared_ptr是一个最像指针的“智能指针“，是boost.smart_ptr中最有价值、最重要的组成部分，也是最有用的,boost库的许多组件甚至还包括其它一些领域的智能指针都使用了shared_ptr。
shared_ptr非常有价值、非常重要、非常有用。
shared_ptr与scoped_ptr一样包装了new操作符在堆上分配的动态对象，但它实现的是引用计数型的智能指针，可以被自由拷贝和赋值，在任意的其它地方共享。当没有代码使用（引用）它时才删除被包装的动态分配的对象。shared_ptr可以安全地放在标准容器中，并弥补了auto_ptr因为转移语义而不能把指针作为STL容器元素的缺陷。
在C++历史上出现过无数的引用计数智能指针实现，但没有一个比得上boost::shared_ptr，在过去、现在和将来，它都是最好的。
shared_ptr的名字表明了它与scoped_ptr的主要不同：它是可以被安全共享的---shared_ptr是一个”全功能“的类，有着正常的拷贝、赋值语义，也可以进行shared_ptr间的比较，是”最智能“的智能指针。
shared_ptr 有多种形式的构造函数，应用于各种可能的情形：
 + 无参的shared_ptr()创建一个持有空指针的shared_ptr;
 + shared_ptr(Y *p)获得指向类型T的指针p的管理权，同时引用计数转为1。这个构造函数要求Y类型必须能够转换为T类型；
 + shared_ptr(shared_ptr const &r) 从另一个shared_ptr获得指针的管理权，同时引用计数加1，结果是两个shared_ptr共享一个指针的管理权。
 + shared_ptr(std::auto_ptr<Y> &r) 从一个auto_ptr获得指针的管理权，引用计数置为1，同时auto_ptr自动失去管理权；
 + operator=赋值操作符可以从另外一个shared_ptr或auto_ptr获得指针的管理权，其行为同构造函数；
 + shared_ptr(Y* p, D d)行为类似shared_ptr(Y* p)，但使用了参数D指定了析构时的定制删除器，而不是简单的delete。
shared_ptr的reset()函数的行为与scoped_ptr也不尽相同，它的作用是将引用计数减1，停止对指针的共享，除非引用计数为0，否则不会发生删除操作。带参数的reset()则类似相同形式的构造函数，原指针引用计数减1的同时修改为管理另一个指针。
shared_ptr有两个专门的函数来检查引用计数。unique()在shared_ptr是指针的唯一所有者进返回true（这时shared_ptr的行为类似auto_ptr或scoped_ptr)，use_count()返回当前指针的引用计数。
shared_ptr还支持比较运算，可以测试两个shared_ptr的相等或不等，比较基于内部保存的指针，相当于a.get() == b.get()。同时提供了operator<以用于标准关联容器。
在编写基于虚函数的多态代码时指针的类型转换很有用，比如把一个基类指针转型为一个子类指针或者反过来。但对于shared_ptr不能使用诸如 static_cast<T*>(p.get())的形式，这将导致转型后的指针无法再被shared_ptr正确管理。为了支持这样的用法，shared_ptr提供了类似 的转型函数 static_pointer_cast<T>()、const_pointer_cast<T>()和 dynamic_pointer_cast<T>()，它们与标准的转型操作符 static_cast<T>、const_cast<T> 和 dynamic_cast<T>类似，但返回的是转型后的shared_ptr。

smart_ptr还提供了一个make_shared自由工厂函数来消除显式的new调用。此外，smart_ptr库还提供了一个allocate_shared()，它比make_shared()多接受一个定制的内存分配器类型参数，其他方面相同。

桥接模式非常有用，它可任意改变具体的实现而外界对此一无所知，也减小了源文件之间的编译依赖，使程序获得了更多的灵活性。而shared_ptr是实现它的最佳工具之一，它解决了指针的共享和引用计数问题。

shared_ptr<void>能够存储 void* 型的指针，而 void* 型指针可以指向任意类型，因此shared_ptr<void> 就像是一个泛型的指针容器，拥有容纳任意类型的能力。但将指针存储为(void*)同时也丧失了原来的类型信息，为了在需要的时候正确使用，可以使用 static_pointer_cast<T> 等转型函数重新转为原来的指针。但这涉及到运行时动态类型转换，它会使代码不够安全，建议最好不要这样用。
由于空指针可以是任何指针类型，因此shared_ptr<void>还可以实现退出作用域时调用任意函数。shared_ptr<void>存储了一个空指针，并指定了删除器是操作 void* 的一个函数，因此当它析构时会自动调用函数any_func()，从而执行任意我们想做的工作。


#shared_array
类似shared_ptr，它包装了new[]操作符在堆上分配的动态数组，同样使用引用计数机制为动态数组提供了一个代理，可以程序的生命周期里长期存在，直到没有任何引用后才释放内存。
shared_array能力有限，多数情况下它可以用shared_ptr<std::vector>或者 std::vector<shared_ptr>来代替，这两个方案具有更好的安全性和更多的灵活性，而所付出的代价几乎可以忽略不计。

#scoped_ptr
一旦scoped_ptr获取了对象的管理权，你就无法再从它那里取回来
scoped_ptr的reset()功能是重置：删除原来的指针，再保存新的指针值p。如果p是空指针，那么scoped_ptr将不再持有任何指针。一般情况下，reset()不应该被调用，因为它违背了scoped_ptr的本意---资源应该一直由scoped_ptr自己自动管理。

成员函数get()返回scoped_ptr内部保存的原始指针，可以用在某些必须是原始指针的场景（如底层C接口）。但使用时必须小心，这将使原始指针脱离scoped_ptr的控制！不能对这个指针做delete操作，否则 scoped_ptr析构时会对已删除的指针进行删除操作，发生未定义行为。

使用scoped_ptr会带来两个好处：一是使代码变得清晰简单，而简单意味着更少的错误；二是它并没有增加多余的操作，安全的同时保证了效率，并可以获得与原始指针同样的速度。

scoped_ptr的用法与auto_ptr几乎一样，大多数情况下它可以与auto_ptr互换，它也可以从一个auto_ptr获得指针的管理权（同时auto_ptr失去管理权）。
scoped_ptr也具有auto_ptr同样的“缺陷”---不能用作容器的基本元素，但原因不同：auto_ptr是因为它的转移语义，而scoped_ptr则是因为不支持拷贝和赋值，不符合容器对元素类型的要求。
scoped_ptr与auto_ptr的根本性区别在于指针的所有权，auto_ptr特意被设计成所有是可转移的，可以在函数之间传递，同一时刻只能有一个auto_ptr管理指针。用意是好的，但是容易引发错误。而scoped_ptr把拷贝和赋值构造函数都声明为私有的，拒绝了所有权转让。

scoped_array很像scoped_ptr，它包装了new []操作符在堆上分配的动态数组，为动态数组提供了一个代理，保证可以正确的释放内存. 它弥补了标准库中没有指向指针数组的智能指针的缺憾。
scoped_array的接口和功能几乎是与scoped_ptr相同的，主要特点如下：
 + 构造函数接受的指针p必须是new[]的结果，而不能是new表达式的结果
 + 没有*、->操作符重载，因为scoped_array持有的不是普通指针，所以也不能用“数组首地址+N”的方式访问
 + 析构函数使用delete[]释放资源，而不是delete
 + 提供operator[]操作符重载，可以像普通数组一样用下标访问元素，但是不提供数组索引范围检查，要小心
 + 没有begin()、end()等类似容器的迭代器操作函数 
 !!!!!!=======>
 在需要动态数组的情况下我们应该使用std::vector，它比scoped_array提供了更多的灵活性，而只付出了很小的代价，而且vector有丰富的成员函数来操纵数据。
 除非对性能有非常严格的要求，或者编译器不支持标准库（如某些嵌入式系统），不推荐使用scoped_array，它只是为了与老式C代码风格兼容而使用的类，它的出现往往意味着你的代码中存在着隐患。
<=========!!!!!!

# weak_ptr
weak_ptr 是为配合 shared_ptr 而引入的一种智能指针，它更像是 shared_ptr 的一个助手而不是智能指针，因为它不具有普通指针的行为，没有重载 operator*和->。它的最大作用在于协助 shared_ptr 工作，像旁观者那样观测资源的使用情况。
它可以从一个shared_ptr 或者另一个weak_ptr 对象构造，获得资源的观测权。 但weak_ptr没有共享资源，它的构造不会引起指针引用计数的增加。同样，在weak_ptr析构时也不会导致引用计数减少，它只是一个静静的观察者。
weak_ptr没有重载operator->和*，这是特意的，因为它不共享指针，不能操作资源，这正是它”弱“的原因。但它可以使用一个非常重要的成员函数lock()从被观测的 shared_ptr获得一个可用的shared_ptr对象，从而操作资源。但当expired() == true 的时候，lock() 函数将返回一个存储空指针的 shared_ptr。



