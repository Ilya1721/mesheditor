#pragma once

#include <concepts>
#include <functional>

namespace MeshCore
{
    template <typename T>
    concept Pointer = std::is_pointer_v<T>;

    template <typename T>
    concept Tree = requires(const T& tree)
    {
        { tree.getChildren() } -> std::ranges::range;
        { (*tree.getChildren().begin()).get() } -> Pointer;
    };

    template <Tree T>
    using TreeNodeFunction = std::function<void(T*)>;

    template<Tree T>
    class TreeWalker
    {
    public:
        TreeWalker(T* root, bool invokeFunctionForRoot = false) :
            mRoot(root),
            mInvokeFunctionForRoot(invokeFunctionForRoot)
        {};

        void forEach(const TreeNodeFunction<T>& function)
        {
            depthFirstTraversal(mRoot, function);
        }

    private:
        void depthFirstTraversal(T* node, const TreeNodeFunction<T>& function)
        {
            if (!node)
            {
                return;
            }

            if (mInvokeFunctionForRoot || node != mRoot)
            {
                function(node);
            }

            for (auto& child : node->getChildren())
            {
                depthFirstTraversal(child.get(), function);
            }
        }

    private:
        T* mRoot;
        bool mInvokeFunctionForRoot;
    };
}