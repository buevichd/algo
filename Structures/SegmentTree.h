template <class T>
class IdentityConverter {
public:
    T operator()(const T& obj) {
        return obj;
    }
};

/**
 * @tparam ElementType - type of stored elements
 * @tparam ResultType - type stored in the tree
 * @tparam MergeFunctor - functor ResultType(ResultType, ResultType)
 * @tparam ConvertFunctor - functor ResultType(ElementType)
 */
template <class ElementType,
    class ResultType,
    class MergeFunctor,
    class ConvertFunctor = IdentityConverter<ElementType>>
class SegmentTree {
public:
    SegmentTree(std::vector<ElementType> data, MergeFunctor merge_functor,
        ResultType identity = ResultType(),
        ConvertFunctor convert_functor = IdentityConverter<ElementType>())

        : data_(std::move(data)),
        tree_(4 * data_.size()),
        addend_(4 * data_.size(), identity),
        merge_functor_(merge_functor),
        convert_functor_(convert_functor),
        identity_(identity) {

        static_assert(std::is_integral<ElementType>::value);

        if (!data_.empty()) {
            Build(0, 0, data_.size());
        }
    }

    ResultType GetValue(size_t left, size_t right = SIZE_MAX) {
        if (right == SIZE_MAX) {
            right = left + 1;
        }
        return GetValue(0, 0, data_.size(), left, right);
    }

    void SetElement(size_t position, ElementType value) {
        SetElement(0, 0, data_.size(), position, value);
    }

    void SetSegment(size_t left, size_t right, ResultType value) {
        SetSegment(0, 0, data_.size(), left, right, value);
    }

private:
    std::vector<ElementType> data_;
    std::vector<ResultType> tree_;
    std::vector<ResultType> addend_;
    MergeFunctor merge_functor_;
    ConvertFunctor convert_functor_;
    ResultType identity_;

    void Build(size_t vertex, size_t left, size_t right) {
        if (left + 1 == right) {
            tree_[vertex] = convert_functor_(data_[left]);
        } else {
            size_t middle = (right + left) / 2;
            Build(GetLeftChild(vertex), left, middle);
            Build(GetRightChild(vertex), middle, right);
            tree_[vertex] = merge_functor_(tree_[GetLeftChild(vertex)], tree_[GetRightChild(vertex)]);
        }
    }

    ResultType GetValue(size_t vertex, size_t vertex_left, size_t vertex_right,
                         size_t left, size_t right) {

        if (left >= right) {
            return identity_;
        }
        if (vertex_left == left && vertex_right == right) {
            return tree_[vertex];
        }
        size_t vertex_middle = (vertex_left + vertex_right) / 2;
        return merge_functor_(GetValue(GetLeftChild(vertex), vertex_left, vertex_middle, left, std::min(right, vertex_middle)),
                              GetValue(GetRightChild(vertex), vertex_middle, vertex_right, std::max(left, vertex_middle), right));
    }

    void SetElement(size_t vertex, size_t vertex_left, size_t vertex_right, size_t position,
                    ElementType new_value) {
        if (vertex_left + 1 == vertex_right) {
            tree_[vertex] = convert_functor_(new_value);
        } else {
            size_t vertex_middle = (vertex_left + vertex_right) / 2;
            if (position < vertex_middle) {
                SetElement(GetLeftChild(vertex), vertex_left, vertex_middle, position, new_value);
            } else {
                SetElement(GetRightChild(vertex), vertex_middle, vertex_right, position, new_value);
            }
            tree_[vertex] = merge_functor_(tree_[GetRightChild(vertex)], tree_[GetLeftChild(vertex)]);
        }
    }

    void SetSegment(size_t vertex, size_t vertex_left, size_t vertex_right, size_t left,
                    size_t right,
                    ResultType value) {

        if (left >= right) {
            return;
        }
        if (vertex_left == left && vertex_right == right) {
            addend_[vertex] = value;
        } else {
            size_t vertex_middle = (vertex_left + vertex_right) / 2;
            SetSegment(GetLeftChild(vertex), vertex_left, vertex_middle, left, std::min(vertex_middle, right), value);
            SetSegment(GetRightChild(vertex), vertex_middle, vertex_right, std::max(left, vertex_middle), right, value);
        }
    }

    size_t GetLeftChild(size_t vertex) const {
        return 2 * vertex + 1;
    }

    size_t GetRightChild(size_t vertex) const {
        return 2 * vertex + 2;
    }
};

template <class ElementType, class MergeFunctor>
SegmentTree<ElementType, ElementType, MergeFunctor, IdentityConverter<ElementType>>
    MakeSegmentTree(std::vector<ElementType> data, MergeFunctor merge_functor, ElementType identity) {

    return SegmentTree<ElementType, ElementType, MergeFunctor, IdentityConverter<ElementType>>(
        data, merge_functor, identity, IdentityConverter<ElementType>());
}

template <class ElementType, class ResultType, class MergeFunctor, class ConvertFunctor>
SegmentTree<ElementType, ResultType, MergeFunctor, ConvertFunctor> MakeSegmentTree(
    std::vector<ElementType> data, MergeFunctor merge_functor, ResultType identity, ConvertFunctor convert_functor) {

    return SegmentTree<ElementType, ResultType, MergeFunctor, ConvertFunctor>(
        data, merge_functor, identity, convert_functor);
}

using SimpleSegmentTree = SegmentTree<int, int, std::function<int(int,int)>, IdentityConverter<int>>;
