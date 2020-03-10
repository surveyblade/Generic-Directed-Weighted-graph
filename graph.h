#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <map>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <utility>
#include <tuple>

namespace gdwg {

	// requires Relation<Less<T>, T>()
	// struct CompareByValue {
	//   bool operator()(const std::smart_ptr<T>& lhs, const std::smart_ptr<T>& rhs) const {
	//     return *lhs < *rhs;
	//   }
	// };


	template<typename N, typename E>
	class Graph {
	public:
		//  struct Node {
		//    N val;
		//  };
		//
		//  struct Edge {
		//    E edge;
		//  };
		struct comparestruct {
			bool operator()(std::pair<N, E>& a, std::pair<N, E>& b) const {
				if (a.first == b.first) {
					return a.second < b.second;
				}
				return a.first < b.first;
			}
		};
		// Constructor
		Graph() {}
		Graph(typename std::vector<N>::const_iterator begin,
			typename std::vector<N>::const_iterator end) noexcept;
		Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator begin,
			typename std::vector<std::tuple<N, N, E>>::const_iterator end) noexcept;
		Graph(std::initializer_list<N> n) noexcept;
		Graph(const Graph& g) noexcept;
		Graph(Graph&& g) noexcept;

		~Graph<N, E>() = default;

		// Operation
		Graph& operator=(const gdwg::Graph<N, E>& g) noexcept;
		Graph& operator=(gdwg::Graph<N, E>&& g) noexcept;

		// Methods
		bool InsertNode(const N& val) noexcept;
		bool InsertEdge(const N& src, const N& dst, const E& w);
		bool DeleteNode(const N& n) noexcept;
		bool Replace(const N& oldData, const N& newData);
		void MergeReplace(const N& oldData, const N& newData);
		void Clear();
		bool IsNode(const N& val);
		bool IsConnected(const N& src, const N& dst);
		std::vector<N> GetNodes();
		std::vector<N> GetConnected(const N& src);
		std::vector<E> GetWeights(const N& src, const N& dst);
		bool erase(const N& src, const N& dst, const E& w) noexcept;

		// Friends
		friend bool operator==(const gdwg::Graph<N, E>& lhs, const gdwg::Graph<N, E>& rhs) noexcept {
			Graph newlhs = lhs;
			Graph newrhs = rhs;
			std::sort(newlhs.nodes_.begin(), newlhs.nodes_.end());
			std::sort(newrhs.nodes_.begin(), newrhs.nodes_.end());
			for (auto i = newlhs.edges_.begin(); i != newlhs.edges_.end(); ++i) {
				std::sort(i->second.begin(), i->second.end());
			}
			for (auto i = newrhs.edges_.begin(); i != newrhs.edges_.end(); ++i) {
				std::sort(i->second.begin(), i->second.end());
			}
			if (newlhs.nodes_ == newrhs.nodes_) {
				if (newlhs.edges_ == newrhs.edges_) {
					return true;
				}
			}
			return false;
		}

		friend bool operator!=(const gdwg::Graph<N, E>& lhs, const gdwg::Graph<N, E>& rhs) noexcept {
			Graph newlhs = lhs;
			Graph newrhs = rhs;
			std::sort(newlhs.nodes_.begin(), newlhs.nodes_.end());
			std::sort(newrhs.nodes_.begin(), newrhs.nodes_.end());
			for (auto i = newlhs.edges_.begin(); i != newlhs.edges_.end(); ++i) {
				std::sort(i->second.begin(), i->second.end());
			}
			for (auto i = newrhs.edges_.begin(); i != newrhs.edges_.end(); ++i) {
				std::sort(i->second.begin(), i->second.end());
			}
			if (newlhs.nodes_ == newrhs.nodes_) {
				if (newlhs.edges_ == newrhs.edges_) {
					return false;
				}
			}
			return true;
		}

		//  void swap(N& a, N& b)
		//  {
		//    auto temp = a;
		//    a = b;
		//    b = temp;
		//  }
		//  void swap(E& a, E& b)
		//  {
		//    auto temp = a;
		//    a = b;
		//    b = temp;
		//  }
		//
		//  void BubbleSort(std::vector<std::pair<N, E>> v) {
		//    for (int i = 0; i < static_cast<int>(v.size()); i++) {
		//      for (int j = i+1; j < (static_cast<int>(v.size()) - i - 1); j++) {
		//        if (v[i].first > v[j].first) {
		//          swap(v[i], v[j]);
		//        }
		//      }
		//    }
		//  }

		friend std::ostream& operator<<(std::ostream& os, const gdwg::Graph<N, E>& g) {
			Graph gg = g;
			for (auto i = gg.edges_.begin(); i != gg.edges_.end(); ++i) {
				//  g.BubbleSort(i->second);
				std::sort(i->second.begin(), i->second.end());
			}
			for (auto i = gg.edges_.begin(); i != gg.edges_.end(); ++i) {
				os << i->first << " (\n";
				if (!i->second.empty()) {
					for (int m = 0; m < static_cast<int>(i->second.size()); m++) {
						os << "  " << i->second[m].first << " | " << i->second[m].second << "\n";
					}
				}
				os << ")\n";
			}
			return os;
		}
	private:
		// std::set<std::smart_ptr<Node>, CompareByValue<Node>> nodes_;
		// std::map<std::smart_ptr<Node>, ???, CompareByValue<Node>> nodes_;
		// std::vector<std::unique_ptr<N[]>> nodes_;
		std::vector<N> nodes_;
		// std::map<shared_ptr<N>, std::set<std::pair<weak_ptr<N>, unique_ptr<E>>> edges_;
		std::map<N, std::vector<std::pair<N, E>>> edges_;
	};

	// Define your graph_iterator here
	// Define your graph_const_iterator here

}  // namespace gdwg

#include "graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_


