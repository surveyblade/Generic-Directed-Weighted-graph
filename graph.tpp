#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <string>



// Constructor

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator begin,
                         typename std::vector<N>::const_iterator end) noexcept {
  for (auto i = begin; i != end; ++i) {
    nodes_.push_back(*i);
  }
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator begin,
                         typename std::vector<std::tuple<N, N, E>>::const_iterator end) noexcept {
  for (auto i = begin; i != end; ++i) {
    nodes_.push_back(std::get<0>(*i));
    nodes_.push_back(std::get<1>(*i));
    edges_[std::get<0>(*i)].push_back(std::make_pair(std::get<1>(*i), std::get<2>(*i)));
  }
  nodes_.erase(std::unique(nodes_.begin(), nodes_.end()), nodes_.end());
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> n) noexcept {
  for (const auto& i : n) {
    nodes_.push_back(i);
  }
}

// Copy constructor
template<typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E>& g) noexcept {
  nodes_ = g.nodes_;
  edges_ = g.edges_;
}

// Move constructor
template<typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&& g) noexcept {
  nodes_ = std::move(g.nodes_);
  edges_ = std::move(g.edges_);
}

// Operations
// Copy Assignment
template<typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(const gdwg::Graph<N, E>& g) noexcept {
  nodes_ = g.nodes_;
  edges_ = g.edges_;
  return *this;
}

// Move Assignment
template<typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(gdwg::Graph<N, E>&& g) noexcept {
  nodes_ = std::move(g.nodes_);
  edges_ = std::move(g.edges_);
  return *this;
}


// Methods
template<typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) noexcept {
  if (std::find(nodes_.begin(), nodes_.end(), val) != nodes_.end()) {
    return false;
  }
  nodes_.push_back(val);
  return true;
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
  auto result1 = std::find(std::begin(nodes_), std::end(nodes_), src);
  auto result2 = std::find(std::begin(nodes_), std::end(nodes_), dst);
  if (result1 != std::end(nodes_) && result2 !=std::end(nodes_)) {
    edges_[src].push_back(std::make_pair(dst, w));
    return true;
  } else {
    throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");
  }
  return false;
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& n) noexcept {
  // if n is in nodes_
  if (std::find(nodes_.begin(), nodes_.end(), n) != nodes_.end()) {
    nodes_.erase(std::remove(nodes_.begin(), nodes_.end(), n), nodes_.end());
    // delete nodes in edges
    for (auto i = edges_.begin(); i != edges_.end(); ++i) {
      // if n is the scr node
      if (i->first == n) {
        edges_.erase(n);
      }
      // if n is the dst node
      for (auto m = 0; m < static_cast<int>(i->second.size()); m++) {
        if (i->second[m].first == n) {
          i->second.erase(std::remove(i->second.begin(), i->second.end(), i->second[m]), i->second.end());
        }
      }
      return true;
    }
  }
  return false;
}


template<typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N& oldData, const N& newData) {
  // if oldData exists
  if (std::find(nodes_.begin(), nodes_.end(), oldData) != nodes_.end()) {
    nodes_.erase(std::remove(nodes_.begin(), nodes_.end(), oldData), nodes_.end());
    nodes_.push_back(newData);
    for (auto i = edges_.begin(); i != edges_.end(); ++i) {
      // if oldData is the src node
      auto nodeHandler = edges_.extract(oldData);
      nodeHandler.key() = newData;
      edges_.insert(std::move(nodeHandler));
      // if oldData is the dst node
      for (auto m = 0; m < static_cast<int>(i->second.size()); m++) {
        if (i->second[m].first == oldData) {
          i->second[m].first = newData;
        }
      }
    }
    return true;
  } else {
    throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
  }
  return false;
}

template<typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
  auto result1 = std::find(std::begin(nodes_), std::end(nodes_), oldData);
  auto result2 = std::find(std::begin(nodes_), std::end(nodes_), newData);
  if (result1 != std::end(nodes_) && result2 !=std::end(nodes_)) {
    nodes_.erase(std::remove(nodes_.begin(), nodes_.end(), oldData), nodes_.end());
    for (auto i = edges_.begin(); i != edges_.end(); ++i) {
      // if oldData is the src node
      if (i->first == oldData) {
        // old,<old,weight> instance
        for (auto m = 0; m < static_cast<int>(i->second.size()); m++) {
          if (i->second[m].first == oldData) {
            i->second.erase(std::remove(i->second.begin(), i->second.end(), i->second[m]), i->second.end());
          }
        }
        auto nodeHandler = edges_.extract(oldData);
        nodeHandler.key() = newData;
        edges_.insert(std::move(nodeHandler));
      }
      // if oldData is the dst node
      for (auto m = 0; m < static_cast<int>(i->second.size()); m++) {
        if (i->second[m].first == oldData) {
          i->second[m].first = newData;
        }
      }
    }
  } else {
    throw std::runtime_error("Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
  }
}

template<typename N, typename E>
void gdwg::Graph<N, E>::Clear() {
  nodes_.clear();
  edges_.clear();
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) {
  if (std::find(nodes_.begin(), nodes_.end(), val) != nodes_.end()) {
    return true;
  }
  return false;
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& src, const N& dst) {
  auto result1 = std::find(std::begin(nodes_), std::end(nodes_), src);
  auto result2 = std::find(std::begin(nodes_), std::end(nodes_), dst);
  if (result1 != std::end(nodes_) && result2 !=std::end(nodes_)) {
    for (auto i = edges_.begin(); i != edges_.end(); ++i) {
      if (i->first == src) {
        for (auto m = 0; m < static_cast<int>(i->second.size()); m++) {
          if (i->second[m].first == dst) {
            return true;
          }
        }
      }
    }
  } else {
    throw std::runtime_error("Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }
  return false;
}

template<typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes() {
  sort(nodes_.begin(), nodes_.end());
  return nodes_;
}

template<typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N& src) {
  if (std::find(nodes_.begin(), nodes_.end(), src) != nodes_.end()) {
    std::vector<N> connected_nodes;
    for (auto i = edges_.begin(); i != edges_.end(); ++i) {
      if (i->first == src) {
        for (auto m = 0; m < static_cast<int>(i->second.size()); m++) {
          connected_nodes.push_back(i->second[m].first);
        }
      }
    }
    std::sort(connected_nodes.begin(), connected_nodes.end());
    connected_nodes.erase(std::unique(connected_nodes.begin(), connected_nodes.end()),
                          connected_nodes.end());
    return connected_nodes;
  } else {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }
}

template<typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N& src, const N& dst) {
  auto result1 = std::find(std::begin(nodes_), std::end(nodes_), src);
  auto result2 = std::find(std::begin(nodes_), std::end(nodes_), dst);
  if (result1 != std::end(nodes_) && result2 !=std::end(nodes_)) {
    std::vector<E> weights;
    for (auto i = edges_.begin(); i != edges_.end(); ++i) {
      if (i->first == src) {
        for (auto m = 0; m < static_cast<int>(i->second.size()); m++) {
          if (i->second[m].first == dst) {
            weights.push_back(i->second[m].second);
          }
        }
      }
    }
    std::sort(weights.begin(), weights.end());
    return weights;
  } else {
    throw std::out_of_range("Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }
}

template<typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N& src, const N& dst, const E& w) noexcept {
  if (this->IsConnected(src, dst)) {
    for (auto m = 0; m < static_cast<int>(edges_[src].size()); m++) {
      if (edges_[src][m].first == dst && edges_[src][m].second == w) {
        edges_[src].erase(std::remove(edges_[src].begin(), edges_[src].end(), edges_[src][m]), edges_[src].end());
        return true;
      }
    }
  }
  return false;
}

