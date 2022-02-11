/*                                    tab:8
 *
 * main.c - source file for CS225 Addition of Polynomials Program
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE,
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:        ?
 * Version:        2
 * Creation Date:   ?
 * Filename:        main.cpp
 * History:
 *    Klaus-Dieter Schewe    1    11 January 2021
 *        First written.
 *    Hao BAI    2    12 January 2021
 *        Annotations added.
 *
 */


#include <iostream>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

using namespace std;
// C++ requires the namespace "std" to help build a scope for all header files mentioned below.
//  all NEW header files in C++ are required to be in a big package - the creaters chose the
//  name to be "std", which means "standard".
// separately, write as below is also taken as approved, e.g.:
//  using namespace std::cin;
//  using namespace std::cout;

// this programs includes NO explicit template declarations and NO specialization after
//  generalization (SAG). However, some codes are still hard to understand, because it requires
//  much more knowledge than the scope of ECE220. this erges us to learn, but I'm not sure
//  whether it's fair for everyone to check on this during exams.



class Variable {
public:
    char id;
    int exp;
    Variable() { // required by <vector>;
    }
    // here it's pretty important to understand the system of the container <vector>.
    //  later in code we see this line:
    //      vector<Variable> vars;
    //  which means to set up a class of vectors. You can understand by analogy:
    //      vector<int> ints;
    //  this code means the similar, except that int is the built-in data type created by
    //  the top engineers of C++, but Variable is the data type created by yourself.
    //  notice that the class you created can have all the properties you want, but
    //  those who are built-in is already built and you cannot change them. this kind of
    //  objects is call TEMPORARY OBJECTs (they are called TEMP because they don't have names).
    
    // for a vector<Class>, you MUST include a ctor withOUT any input to make an initialization.
    //  that's because when you call the class, a ctor must be called. because you've already
    //  written "Variable(char c, int t)" below, the system will NOT create a default Variable()
    //  again, so you will get an error if you use this class to creat a <vector>. consequently,
    //  it's inevitable to create a "default" ctor to fill in this pitfall.
    
    // below are definitions of the class you build. notice that every definition makes sense,
    //  and it's CRUCIAL to link them with what you write in the functional functions.
    
    Variable(char c, int i) { // ctor
        id = c; exp = i;
    }
    // this part do the basic task: assignment.
    
    bool operator== (const Variable& v) const { // operator overloading.
        return id  == v.id  && exp == v.exp;
    }
    // notice this line later in the text:
    //      else if (pol->vars.size() == 0) // the term has only a coefficient
    //  C++ uses OOP struct, so when the compiler faces an operator, it go to check the
    //  type of the operands first. this line is about vars, which is an object of the class
    //  Variable, so the overloaded operator == will be called by the compiler automatically.
    
    bool operator< (const Variable& v) const { // used by sort();
        return id < v.id;
    }
};

class Term {
public:
    Term() {
        coeff = 0;
    }
    int coeff;
    vector<Variable> vars;
    
    // Notice that the object var with the class Variable is INSIDE the class Term, which
    //  means that we can only get access to var if we have an object with the class Term.
    
    bool operator == (const Term&) const;
    bool operator != (const Term& term) const { // required by <list>
        return !(*this == term);
    }
    
    // <list> requires some overloaded operations (that's its special property), so
    //  you MUST announce them here. For example, look at this line (in the following lines):
    //      return *this != term && (*this < term);
    //  if you delete the overloading operation of !=, you will find that this line will fail
    //  to compile, because this operation uses the above one (!=).
    
    bool operator< (const Term&) const;
    
    // Same problem here. if you delete this line, the following line can NOT be used as well.
    //  The story is, all the operators used here are about overloading. If you don't overload
    //  them, the type is not fitted, so the program will fail to compile.
    
    bool operator> (const Term& term) const {  // required by <list>
        return *this != term && (*this < term);
    }
    
    int min(int n, int m) const {
        return (n < m) ? n : m;
    }
};

class Polynomial {
    // This class is also defined by ourselves. It's not a container.
public:
    Polynomial() { }
    // This line MAY be discarded, because we didn't use any ctor's other than this one.
    //  Thus, the default ctor Polynomial() {} will be called automatically even if we
    //  don't define it ourselves.
    
        Polynomial operator+ (Polynomial&);
    void error(char *s) {
        cerr << s << endl; exit(1);
    }
    
    // Most students feel confused about cerr. It's a PROMPT OUTPUT type with no buffer to
    //  store - it goes out immediately. As we've learnt in ECE220, a buffer stores a BUNCH
    //  of outputs, and put them on the screen at the same time - that's std::cout. But cerr
    //  makes an output immediately without going through buffer - so it's much safer if you
    //  face any problems.
    
    // For exit(), it's something like return(), but it exits IMMEDIATELY. As we know, if the
    //  program tree is
    //          main()
    //          - division()
    //          - - division_error()
    //          - multiply()
    //  Then if you want to exit immediately if you see an error in division_error,
    //  you need to use exit() instead of return, because return goes back to the upper
    //  function, but exit ends the program immediately.
    
private:
    list<Term> terms;
    
    // The class 'Polynomial' creates a list container terms inside itself. This operation
    //  is FREQUENTLY used when designing programs in C++ Lang. Now you can go back to the container
    //  <Term>. You can observe that it includes an object of another type, namely, vars.
    //  This kind of design is frequently used because it make all the upper classes's properties visible
    //  to the one you're compiling at.
     
    friend istream& operator>> (istream& in, Polynomial& polyn) {
        
        // notice that this function used a method called "pass by reference".
        //  When a parameter is passed by reference, the caller and the callee use the same variable for the parameter.
        //  If the callee modifies the parameter variable, the effect is visible to the caller’s variable. __from SO__
        // also notice that this function used a method called "return by reference".
        //  When a function returns by reference, the returned "thing" must be delivered into the function as a parameter.
        //  Otherwise this "thing" is a local variable. Thus, the local variable inside the functions will DIE after
        //  its reference is passed out, and the caller will get nothing but a corpse. In this case, you MUST use return
        //  by value.
        // thus, why do we still use "return by reference"?
        //  Because it saves storage. Note that return by reference only takes 4 bytes for the return "thing", but return
        //  by value takes AT LEAST 4 bytes depending on the returned "thing".
        
        char ch, sign, coeffUsed, id;
        int exp;
        Term term;
        // this class has an inline object too - it declares an inline object term of class Term inside
        //  the class Polynomial.
        
        in >> ch;
        
        // the variable "in" is passed in by the caller as a parameter, and is shifted right for units of ch.
        // notice that it's of type &istream.
        //  the type "istream" is a type with a brother "ostream", and both of them are defined insidide the file
        //  <iostream> in the std C++ library.
        
        
        
        while (true) {
            coeffUsed = 0;
            if (!isalnum(ch) && ch != ';' && ch != '-' && ch != '+')
                 polyn.error("Wrong character entered2");
            sign = 1;
            while (ch == '-' || ch == '+') { // first get sign(s) of Term
                 if (ch == '-')
                       sign *= -1;
                 ch = in.get();
                 if (isspace(ch))
                       in >> ch;
            }
            if (isdigit(ch)) {              // and then its coefficient;
                
                 in.putback(ch);
                
                // .putback() means to put an element BACK to the istream.
                //  this is a function created in istream. when you put something back, it goes back to the
                //  istream, just like you pick something from the box, and you put it back now.
                // notice that this is different from the function push_back.
                //  push_back is a function inside the containers <vector> & <list> & <deque>. it means to
                //  add (push) one variable at the BACK the container.
                
                 in >> term.coeff;
                
                // .coeff is public to the outside world in the class Term. So we can make use of it here.
                
                 ch = in.get();
                
                // this line requires some knowledge of iostream. this is NOT LIKELY to show in CS225, and you are
                //  not required to command this part in a considerable period of time.
                
                // as we've seen that, in the codes above, we've "get" one char from the istream. also, in the if
                //  statement, we've "putback" that char to the istream, and now we draw it again
                //  (after "in >> term.coeff;").
                
                 term.coeff *= sign;
                 coeffUsed = 1;
            }
            else term.coeff = sign;
            
        int i;
            
            // this is a really good program habit. when you see a backtab in a big program, it means that
            //  this is an declaration / initialization of a new variable. programmers tend to behave more
            //  and more special in his own styles, but there ARE something to choose as "example".
            
            for (int i = 0; isalnum(ch); i++) { // process this term:
                
                // isalnum == isalpha && isdigit
                //  we've learnt this similar to python3 in CS101.
                
                id = ch;                    // get a variable name
                ch = in.get();
                if (isdigit(ch)) {          // and an exponent (if any);
                     in.putback(ch);
                     in >> exp >> ch;
                }
                else exp = 1;
                term.vars.push_back(Variable(id,exp));
            }
            polyn.terms.push_back(term);    // and include it in the linked list;
            
            term.vars.resize(0);
            
            // this is a pretty easy function. e.g.
            //  The list holds: 1 2 3
            //  After resize up to 5: 1 2 3 0 0
            //  After resize down to 2: 1 2
            
            if (isspace(ch))
            
                // isspace() is a function with a single char as input.
                //  notice that isspace() is a built in function in C library.
            
                 in >> ch;
            if (ch == ';')                  // finish if a semicolon is entered;
                 if (coeffUsed || i > 0)
                      break;
                 else polyn.error("Term is missing");  // e.g., 2x - ; or just ';'
            else if (ch != '-' && ch != '+')           // e.g., 2x  4y;
                 
                polyn.error("wrong character entered");
                
            // this is an outstanding design. the class Polynomial creates a function
            //  void error(char *s) {
            //  cerr << s << endl; exit(1);
            //  }
            // thus, when we pass a string into the function, it uses the address of the first char as the parameter,
            //  so we'll get the "cerr << s << endl;" part printed and exit at once.
            
        }
        for (list<Term>::iterator it = polyn.terms.begin(); it != polyn.terms.end(); it++)
        
        // Notice that if you want to initialize an iterator, you MUST announce the type of
        //  container it belongs to. An iterator of <list> is STRICTLY different from
        //  an iterator of <array>.
        
            if (it->vars.size() > 1)
                sort(it->vars.begin(),it->vars.end());
        return in;
    }
    
    friend ostream& operator<< (ostream& out, const Polynomial& polyn) {
        int afterFirstTerm = 0, i;
        for (list<Term>::const_iterator pol = polyn.terms.begin(); pol != polyn.terms.end(); pol++) {
            out.put(' ');
            if (pol->coeff < 0)             // put '-' before polynomial
                 out.put('-');              // and between terms (if needed);
            else if (afterFirstTerm)        // don't put '+' in front of
                 out.put('+');              // polynomial;
            afterFirstTerm++;
            if (abs(pol->coeff) != 1)       // print a coefficient
                 out << ' ' << abs(pol->coeff);// if it is not 1 nor -1, or
            else if (pol->vars.size() == 0) // the term has only a coefficient
                 out << " 1";
            else out.put(' ');
            for (i = 1; i <= pol->vars.size(); i++) {
                 out << pol->vars[i-1].id;       // print a variable name
                 if (pol->vars[i-1].exp != 1)    // and an exponent, only
                      out << pol->vars[i-1].exp; // if it is not 1;
            }
        }
        
        // The part above is (perhaps) the easiest part inside the whole file.
        // It uses a variable of type ostream& called "out" to perform. .put simply means add something into it.
        
        out << endl;
        return out;
    }
};

// two terms are equal if all varibles are the same and
// corresponding variables are raised to the same powers;
// the first cell of the node containing a term is excluded
// from comparison, since it stores coefficient of the term;

bool Term::operator== (const Term& term) const {
    int i;
    for (i = 0; i < min(vars.size(),term.vars.size()) &&
                    vars[i] == term.vars[i]; i++);
    return i == vars.size() && vars.size() == term.vars.size();
}

bool Term::operator< (const Term& term2) const { // used by sort();
    if (vars.size() == 0)
        return false;           // *this is just a coefficient;
    else if (term2.vars.size() == 0)
        return true;            // term2 is just a coefficient;
    for (int i = 0; i < min(vars.size(),term2.vars.size()); i++)
    
    // min() is the function defined inside the class Term. The functionality is exactly the same as in our conventions.
    //  however, it's fitted into the Term class to help make the function work. below are the definitions:
    //      int min(int n, int m) const {
    //          return (n < m) ? n : m;
    //      }
    //  as seen, this is nothing more than a normal function - but it uses a form of class function to make more
    //  convenience.
    
    // also notice that this part uses NO "{}" notations. actually, when programming, this is allowed in C++.
    //  that's because "else if" is a follower of "if" - i.e. there's ONLY 1 statement inside the for(){} block.
    // i.e. if you write another line without "else if" or "else", you've already gone out of the for(){} block to code.
    
        if (vars[i].id < term2.vars[i].id)
             return true;       // *this precedes term2;
        else if (term2.vars[i].id < vars[i].id)
             return false;      // term2 precedes *this;
        else if (vars[i].exp < term2.vars[i].exp)
             return true;       // *this precedes term2;
        else if (term2.vars[i].exp < vars[i].exp)
             return false;      // term2 precedes *this;
    
    // bool(ean) is another new type in C++. it has 2 literals: true & false.
    
    return ((int)vars.size() - (int)term2.vars.size() < 0) ? true : false;
    
    // notice that .size returns a "size_type" (as annoucned in the header <vector>), so
    //  it's better to force it into the type "int" (because we need an int here).
    
}


Polynomial Polynomial::operator+ (Polynomial& polyn2) {
    
    // notice that this is an outline operator overloading.
    //  of course, you can also add an "inline" symbol hard before the definition of it,
    //  but it's not required - you just pick your best.
    
    // the most important thing here is you can NOT move this function into the class "definition".
    //  that's because you're using the class "Polynomial" here, but inside the class "definition" you've NOT YET
    //  built the whole class, which means that you can NOT call itself at that time.
    
    Polynomial result;
    list<Term>::iterator p1, p2;
    bool erased;
    for (p1 = terms.begin(); p1 != terms.end(); p1++) // create a new polyn
        result.terms.push_back(*p1);                  // from copies of *this
    for (p1 = polyn2.terms.begin(); p1 != polyn2.terms.end(); p1++) // and
        result.terms.push_back(*p1);                  // polyn2;
    for (p1 = result.terms.begin(); p1 != result.terms.end(); ) {
        for (p2 = p1, p2++, erased = false; p2 != result.terms.end(); p2++)
            if (*p1 == *p2) {             // if two terms are equal (except
                 p1->coeff += p2->coeff;  // for the coefficient), add the
                 result.terms.erase(p2);  // two coefficients and erase
                 if (p1->coeff == 0)      // a redundant term; if the
                     result.terms.erase(p1);// coefficient in retained term
                 erased = true;             // is zero, erase the term as well;
                 break;
            }
        
        // the part above is the basics of an iterator.
        
        if (erased)        // restart processing from the beginning
             p1 = result.terms.begin();  // if any node was erased;
        else p1++;
    }
    
    result.terms.sort();
    
    // .sort is a function inside <list>.
    // notice that all the functions with the form ".func(...)" is inside a container, not from the <algorithm>.
    
    return result;
}

int main() {
    Polynomial polyn1, polyn2;
    cout << "Enter two polynomials, each ended with a semicolon:\n";
    cin  >> polyn1 >> polyn2;
    cout << "The result is:\n" << polyn1 + polyn2;
    return 0;
}

// as you see, the main() part for a C++ program used to be easy. this is the beauty of C++.

1）优点：
     记录数据量很大的时候，处理记录的速度很快，平均操作时间是一个不太大的常数
    2）缺点：
        ①好的哈希函数（good hash function）的计算成本有可能会显著高于线性表或者搜索树在查找时的内部循环成本，所以当数据量非常小的时候，哈希表是低效的
        ②哈希表按照 key 对 value 有序枚举（ordered enumeration， 或者称有序遍历）是比较麻烦的（比如：相比于有序搜索树），需要先取出所有记录再进行额外的排序
        ③哈希表处理冲突的机制本身可能就是一个缺陷，攻击者可以通过精心构造数据，来实现处理冲突的最坏情况。即：每次都出现冲突，甚至每次都出现多次冲突（针对封闭散列的探测），以此来大幅度降低哈希表的性能。这种攻击也被称为基于哈希冲突的拒绝服务攻击（Hashtable collisions as DOS attack）
    
