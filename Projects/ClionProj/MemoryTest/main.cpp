#include <stdio.h>
#include <vector>
#include <map>
#include <string>

class MemoryTestClass {
public:
    std::vector<std::shared_ptr<MemoryTestClass> > children;
    std::weak_ptr<MemoryTestClass> friendBuddy;
    std::string content;
    MemoryTestClass(std::string content) : content(content) {
        printf("%s is Created.\n", content.c_str());
    }

    ~MemoryTestClass() {
        printf("%s is Deleted.\n", content.c_str());
    }

    void addChild(std::shared_ptr<MemoryTestClass> obj) {
        children.push_back(obj);
    }

    void sayHello() {
        std::shared_ptr<MemoryTestClass> friendBuddyShared = this->friendBuddy.lock();
        if (friendBuddyShared) {
            printf("Hello from %s.My friend is %s\n", content.c_str(), friendBuddyShared->content.c_str());
        } else {
            printf("%s lost it's friend.\n", content.c_str());
        }

    }
};

int main(int argc, char **argv) {
    std::shared_ptr<MemoryTestClass> child(new MemoryTestClass("child"));
    printf("%d ref count. \n", child.use_count());
    std::shared_ptr<MemoryTestClass> child2 = child;
    printf("%d ref count. \n", child.use_count());
    {
        std::shared_ptr<MemoryTestClass> child3 = child2;
        printf("%d ref count. \n", child.use_count());
    }
    printf("%d ref count. \n", child.use_count());
    {
        child2.reset();
    }
    printf("%d ref count. \n", child.use_count());

    std::shared_ptr<MemoryTestClass> woman = std::shared_ptr<MemoryTestClass>(new MemoryTestClass("woman"));
    {
        std::shared_ptr<MemoryTestClass> man = std::shared_ptr<MemoryTestClass>(new MemoryTestClass("man"));
        man->friendBuddy = woman;
        woman->friendBuddy = man;
        man->sayHello();
    }
    woman->sayHello();
    return 0;
}