#pragma once

#include <concepts>
#include <functional>

namespace MeshCore
{
    template <typename T>
    concept Tree = requires(const T& tree)
    {
        { tree.getChildren() } -> std::ranges::range;
    };

    template <Tree T>
    using TreeNodeFunction = std::function<void(T*)>;

    template<Tree T>
    class TreeWalker
    {
    public:
        TreeWalker(T* root) :
            mRoot(root)
        {};

        void forEach(const TreeNodeFunction<T>& function)
        {
            depthFirstTraversal(mRoot, function);
        }

    private:
        void depthFirstTraversal(T* node, const TreeNodeFunction<T>& function)
        {
            if (node)
            {
                function(node);
                for (auto& child : node->getChildren())
                {
                    depthFirstTraversal(child, function);
                }
            }
        }

    private:
        T* mRoot;
    };
}