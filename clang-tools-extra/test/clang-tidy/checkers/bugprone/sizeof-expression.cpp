// RUN: %check_clang_tidy %s bugprone-sizeof-expression %t -- -config="{CheckOptions: {bugprone-sizeof-expression.WarnOnSizeOfIntegerExpression: true}}" --

class C {
  int size() { return sizeof(this); }
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: suspicious usage of 'sizeof(this)'
};

#define LEN 8

int X;
extern int A[10];
extern short B[10];

#pragma pack(1)
struct  S { char a, b, c; };

enum E { E_VALUE = 0 };
enum class EC { VALUE = 0 };

bool AsBool() { return false; }
int AsInt() { return 0; }
E AsEnum() { return E_VALUE; }
EC AsEnumClass() { return EC::VALUE; }
S AsStruct() { return {}; }

struct M {
  int AsInt() { return 0; }
  E AsEnum() { return E_VALUE; }
  S AsStruct() { return {}; }
};

int ReturnOverload(int) { return {}; }
S ReturnOverload(S) { return {}; }

template <class T>
T ReturnTemplate(T) { return {}; }

template <class T>
bool TestTrait1() {
  return sizeof(ReturnOverload(T{})) == sizeof(A);
}

template <class T>
bool TestTrait2() {
  return sizeof(ReturnTemplate(T{})) == sizeof(A);
}

template <class T>
bool TestTrait3() {
  return sizeof(ReturnOverload(0)) == sizeof(T{});
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of integer type
}

template <class T>
bool TestTrait4() {
  return sizeof(ReturnTemplate(0)) == sizeof(T{});
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of integer type
}

bool TestTemplates() {
  bool b = true;
  b &= TestTrait1<int>();
  b &= TestTrait1<S>();
  b &= TestTrait2<int>();
  b &= TestTrait2<S>();
  b &= TestTrait3<int>();
  b &= TestTrait3<S>();
  b &= TestTrait4<int>();
  b &= TestTrait4<S>();
  return b;
}

int Test1(const char* ptr) {
  int sum = 0;
  sum += sizeof(LEN);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(K)'
  sum += sizeof(LEN + 1);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(K)'
  sum += sizeof(sum, LEN);
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: suspicious usage of 'sizeof(..., ...)'
  sum += sizeof(AsBool());
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of integer type
  sum += sizeof(AsInt());
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of integer type
  sum += sizeof(AsEnum());
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of integer type
  sum += sizeof(AsEnumClass());
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of integer type
  sum += sizeof(M{}.AsInt());
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of integer type
  sum += sizeof(M{}.AsEnum());
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of integer type
  sum += sizeof(sizeof(X));
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(sizeof(...))'
  sum += sizeof(LEN + sizeof(X));
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(sizeof(...))'
  sum += sizeof(LEN + LEN + sizeof(X));
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(sizeof(...))'
  sum += sizeof(LEN + (LEN + sizeof(X)));
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(sizeof(...))'
  sum += sizeof(LEN + -sizeof(X));
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(sizeof(...))'
  sum += sizeof(LEN + - + -sizeof(X));
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(sizeof(...))'
  sum += sizeof(char) / sizeof(char);
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; both expressions have the same type
  sum += sizeof(A) / sizeof(S);
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; numerator is not a multiple of denominator
  sum += sizeof(char) / sizeof(int);
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; numerator is not a multiple of denominator
  sum += sizeof(char) / sizeof(A);
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; numerator is not a multiple of denominator
  sum += sizeof(B[0]) / sizeof(A);
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; numerator is not a multiple of denominator
  sum += sizeof(ptr) / sizeof(char);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; size of pointer is divided by size of pointed type
  sum += sizeof(ptr) / sizeof(ptr[0]);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; size of pointer is divided by size of pointed type
  sum += sizeof(ptr) / sizeof(char*);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; both expressions have pointer types
  sum += sizeof(ptr) / sizeof(void*);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; both expressions have pointer types
  sum += sizeof(ptr) / sizeof(const void volatile*);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; both expressions have pointer types
  sum += sizeof(ptr) / sizeof(char);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; size of pointer is divided by size of pointed type
  sum += sizeof(int) * sizeof(char);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: suspicious 'sizeof' by 'sizeof' multiplication
  sum += sizeof(ptr) * sizeof(ptr[0]);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: suspicious 'sizeof' by 'sizeof' multiplication
  sum += sizeof(int) * (2 * sizeof(char));
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: suspicious 'sizeof' by 'sizeof' multiplication
  sum += (2 * sizeof(char)) * sizeof(int);
  // CHECK-MESSAGES: :[[@LINE-1]]:29: warning: suspicious 'sizeof' by 'sizeof' multiplication
  if (sizeof(A) < 0x100000) sum += 42;
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: suspicious comparison of 'sizeof(expr)' to a constant
  if (sizeof(A) <= 0xFFFFFFFEU) sum += 42;
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: suspicious comparison of 'sizeof(expr)' to a constant
  return sum;
}

typedef char MyChar;
typedef const MyChar MyConstChar;

int CE0 = sizeof sizeof(char);
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: suspicious usage of 'sizeof(sizeof(...))'
int CE1 = sizeof +sizeof(char);
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: suspicious usage of 'sizeof(sizeof(...))'
int CE2 = sizeof sizeof(const char*);
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: suspicious usage of 'sizeof(sizeof(...))'
int CE3 = sizeof sizeof(const volatile char* const*);
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: suspicious usage of 'sizeof(sizeof(...))'
int CE4 = sizeof sizeof(MyConstChar);
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: suspicious usage of 'sizeof(sizeof(...))'

int Test2(MyConstChar* A) {
  int sum = 0;
  sum += sizeof(MyConstChar) / sizeof(char);
  // CHECK-MESSAGES: :[[@LINE-1]]:30: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; both expressions have the same type
  sum += sizeof(MyConstChar) / sizeof(MyChar);
  // CHECK-MESSAGES: :[[@LINE-1]]:30: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; both expressions have the same type
  sum += sizeof(A[0]) / sizeof(char);
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; both expressions have the same type
  return sum;
}

struct A {
   int array[10];
};

struct B {
  struct A a;
};

void loop_access_elements(int num, struct B b) {
  struct A arr[10];
  char buf[20];

  // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: suspicious usage of 'sizeof' in the loop [bugprone-sizeof-expression]
  for(int i = 0; i < sizeof(arr); i++) {
    struct A a = arr[i];
  }

  // Loop warning should not trigger here, even though this code is incorrect
  // CHECK-MESSAGES: :[[@LINE+2]]:22: warning: suspicious usage of 'sizeof(K)'; did you mean 'K'? [bugprone-sizeof-expression]
  // CHECK-MESSAGES: :[[@LINE+1]]:32: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; numerator is not a multiple of denominator [bugprone-sizeof-expression] 
  for(int i = 0; i < sizeof(10)/sizeof(A); i++) {
    struct A a = arr[i];
  }
    
  // Should not warn here
  for(int i = 0; i < sizeof(arr)/sizeof(A); i++) {}

  // Should not warn here
  for (int i = 0; i < 10; i++) {
    if (sizeof(arr) != 0) {

    }
  }

  for (int i = 0; i < 10; i++) {
    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: suspicious usage of 'sizeof' in the loop [bugprone-sizeof-expression]
    for (int j = 0; j < sizeof(arr); j++) {
    }
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: suspicious usage of 'sizeof' in the loop [bugprone-sizeof-expression]
  for(int j = 0; j < sizeof(b.a.array); j++) {}
  
  // Should not warn here
  for(int i = 0; i < sizeof(buf); i++) {} 

  // Should not warn here
  for(int i = 0; i < (sizeof(arr) << 3); i++) {}
  
  int i = 0;
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: suspicious usage of 'sizeof' in the loop [bugprone-sizeof-expression]
  while(i <= sizeof(arr)) {i++;}
   
  i = 0;
  do {
    i++;
  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: suspicious usage of 'sizeof' in the loop [bugprone-sizeof-expression] 
  } while(i <= sizeof(arr));

  // CHECK-MESSAGES: :[[@LINE+1]]:29: warning: suspicious usage of 'sizeof' in the loop [bugprone-sizeof-expression]
  for(int i = 0, j = 0; i < sizeof(arr) && j < sizeof(buf); i++, j++) {}
}

template <int T>
int Foo() { int A[T]; return sizeof(T); }
// CHECK-MESSAGES: :[[@LINE-1]]:30: warning: suspicious usage of 'sizeof(K)'
template <typename T>
int Bar() { T A[5]; return sizeof(A[0]) / sizeof(T); }
// CHECK-MESSAGES: :[[@LINE-1]]:41: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; both expressions have the same type
int Test3() { return Foo<42>() + Bar<char>(); }

static const char* kABC = "abc";
static const wchar_t* kDEF = L"def";
int Test4(const char A[10]) {
  int sum = 0;
  sum += sizeof(kABC);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(char*)'
  sum += sizeof(kDEF);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(char*)'
  return sum;
}

int Test5() {
  typedef int Array10[10];
  typedef C ArrayC[10];

  struct MyStruct {
    Array10 arr;
    Array10* ptr;
  };
  typedef const MyStruct TMyStruct;
  typedef const MyStruct *PMyStruct;
  typedef TMyStruct *PMyStruct2;

  static TMyStruct kGlocalMyStruct = {};
  static TMyStruct volatile * kGlocalMyStructPtr = &kGlocalMyStruct;

  MyStruct S;
  PMyStruct PS;
  PMyStruct2 PS2;
  Array10 A10;
  C *PtrArray[10];
  C *PC;

  char *PChar;
  int *PInt, **PPInt;
  MyStruct **PPMyStruct;

  int sum = 0;
  sum += sizeof(&S.arr);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(&kGlocalMyStruct.arr);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(&kGlocalMyStructPtr->arr);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(S.arr + 0);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(+ S.arr);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof((int*)S.arr);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type

  sum += sizeof(S.ptr);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(kGlocalMyStruct.ptr);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(kGlocalMyStructPtr->ptr);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type

  sum += sizeof(&kGlocalMyStruct);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(&S);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(MyStruct*);
  sum += sizeof(PMyStruct);
  sum += sizeof(PS);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(PS2);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(&A10);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(PtrArray) / sizeof(PtrArray[1]);
  // CHECK-MESSAGES: :[[@LINE-1]]:29: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  sum += sizeof(A10) / sizeof(PtrArray[0]);
  sum += sizeof(PC) / sizeof(PtrArray[0]);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof()' on an expression of pointer type
  // CHECK-MESSAGES: :[[@LINE-2]]:21: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; both expressions have the same type
  sum += sizeof(ArrayC) / sizeof(PtrArray[0]);
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: suspicious usage of 'sizeof(...)/sizeof(...)'; numerator is not a multiple of denominator

  // These pointers do not point to aggregate types, so they are not reported in this mode:
  sum += sizeof(PChar);
  sum += sizeof(PInt);
  sum += sizeof(PPInt);
  sum += sizeof(PPMyStruct);

  return sum;
}

int Test6() {
  int sum = 0;

  struct S A = AsStruct(), B = AsStruct();
  struct S *P = &A, *Q = &B;
  sum += sizeof(struct S) == P - Q;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic
  sum += 5 * sizeof(S) != P - Q;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic
  sum += sizeof(S) < P - Q;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic
  sum += 5 * sizeof(S) <= P - Q;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic
  sum += 5 * sizeof(*P) >= P - Q;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic
  sum += Q - P > 3 * sizeof(*P);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic
  sum += sizeof(S) + (P - Q);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic
  sum += 5 * sizeof(S) - (P - Q);
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic
  sum += (P - Q) / sizeof(S);
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic
  sum += (P - Q) / sizeof(*Q);
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic

  return sum;
}

static constexpr inline int BufferSize = 1024;

template <typename T>
T next(const T *&Read) {
  T value = *Read;
  Read += sizeof(T);
  return value;
}

void Test7() {
  int Buffer[BufferSize];
  int *P = &Buffer[0];

  const int *P2 = P;
  int V1 = next(P2);
  // CHECK-MESSAGES: :[[@LINE-10]]:8: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic; this scaled value will be scaled again by the '+=' operator
  // CHECK-MESSAGES: :[[@LINE-11]]:8: note: '+=' in pointer arithmetic internally scales with 'sizeof(const int)' == {{[0-9]+}}
  int V2 = next(P2);
  (void)V1;
  (void)V2;

  int *Q = P;
  while (Q < P + sizeof(Buffer)) {
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: suspicious usage of 'sizeof(...)' in pointer arithmetic; this scaled value will be scaled again by the '+' operator
    // CHECK-MESSAGES: :[[@LINE-2]]:16: note: '+' in pointer arithmetic internally scales with 'sizeof(int)' == {{[0-9]+}}
    *Q++ = 0;
  }
}

#ifdef __SIZEOF_INT128__
template <__int128_t N>
#else
template <long N> // Fallback for platforms which do not define `__int128_t`
#endif
bool Baz() { return sizeof(A) < N; }
// CHECK-MESSAGES: :[[@LINE-1]]:31: warning: suspicious comparison of 'sizeof(expr)' to a constant
bool Test8() { return Baz<-1>(); }

void some_generic_function(const void *arg, int argsize);
int *IntP, **IntPP;
C *ClassP, **ClassPP;

void GenericFunctionTest() {
  // The `sizeof(pointer)` checks ignore situations where the pointer is
  // produced by dereferencing a pointer-to-pointer, because this is unlikely
  // to be an accident and can appear in legitimate code that tries to call
  // a generic function which emulates dynamic typing within C.
  some_generic_function(IntPP, sizeof(*IntPP));
  some_generic_function(ClassPP, sizeof(*ClassPP));
  // Using `...[0]` instead of the dereference operator is another common
  // variant, which is also widespread in the idiomatic array-size calculation:
  // `sizeof(array) / sizeof(array[0])`.
  some_generic_function(IntPP, sizeof(IntPP[0]));
  some_generic_function(ClassPP, sizeof(ClassPP[0]));
  // FIXME: There is a third common pattern where the generic function is
  // called with `&Variable` and `sizeof(Variable)`. Right now these are
  // reported by the `sizeof(pointer)` checks, but this causes some false
  // positives, so it would be good to create an exception for them.
  // NOTE: `sizeof(IntP)` is only reported with `WarnOnSizeOfPointer=true`.
  some_generic_function(&IntPP, sizeof(IntP));
  some_generic_function(&ClassPP, sizeof(ClassP));
  // CHECK-MESSAGES: :[[@LINE-1]]:35: warning: suspicious usage of 'sizeof()' on an expression of pointer type
}

int ValidExpressions() {
  int A[] = {1, 2, 3, 4};
  static const char str[] = "hello";
  static const char* ptr[] { "aaa", "bbb", "ccc" };
  typedef C *CA10[10];
  C *PtrArray[10];
  CA10 PtrArray1;

  int sum = 0;
  if (sizeof(A) < 10)
    sum += sizeof(A);
  sum += sizeof(int);
  sum += sizeof(AsStruct());
  sum += sizeof(M{}.AsStruct());
  sum += sizeof(A[sizeof(A) / sizeof(int)]);
  sum += sizeof(&A[sizeof(A) / sizeof(int)]);
  sum += sizeof(sizeof(0));  // Special case: sizeof size_t.
  sum += sizeof(void*);
  sum += sizeof(void const *);
  sum += sizeof(void const *) / 4;
  sum += sizeof(str);
  sum += sizeof(str) / sizeof(char);
  sum += sizeof(str) / sizeof(str[0]);
  sum += sizeof(ptr) / sizeof(ptr[0]);
  sum += sizeof(ptr) / sizeof(*(ptr));
  sum += sizeof(PtrArray) / sizeof(PtrArray[0]);
  // Canonical type of PtrArray1 is same as PtrArray.
  sum = sizeof(PtrArray) / sizeof(PtrArray1[0]);
  // There is no warning for 'sizeof(T*)/sizeof(Q)' case.
  sum += sizeof(PtrArray) / sizeof(A[0]);
  return sum;
}

namespace gh115175 {
template<class T>
int ValidateTemplateTypeExpressions(T t) {
  return sizeof(t.val) / sizeof(t.val[0]);
}
} // namespace gh115175
