/*

  == Explanation and rational of testing ==
  My tesst is basd on every constructor/operation/methods/friend implemented.
  Because I didn't implement those const_iterator methods so I skipped those.
  I believe most of them are crroct.
*/

#include <sstream>
#include <string>
#include <utility>

#include "graph.h"
#include "catch.h"

// TODO(students): Fill this in.
SCENARIO("construcor takes vector") {
  WHEN("Given A Graph") {
    std::vector<std::string> v{"a", "b", "c"};
    gdwg::Graph<std::string, double> b{v.begin(), v.end()};
    THEN("testing the graph construct") {
      REQUIRE(b.IsNode("a"));
      REQUIRE(b.IsNode("b"));
      REQUIRE(b.IsNode("c"));
    }
  }
}
SCENARIO("construcor takes tuple") {
  WHEN("Given A Graph") {
    std::string s1{"a"};
    std::string s2{"b"};
    std::string s3{"c"};
    auto e1 = std::make_tuple(s1, s2, 5.4);
    auto e2 = std::make_tuple(s2, s3, 7.6);
    auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e2};
    gdwg::Graph<std::string, double> b{e.begin(), e.end()};
    THEN("testing the graph construct") {
      REQUIRE(b.IsNode(s1));
      REQUIRE(b.IsNode(s2));
      REQUIRE(b.IsNode(s3));
      REQUIRE(b.IsConnected(s1, s2));
      REQUIRE(b.IsConnected(s2, s3));
    }
  }
}
SCENARIO("construcor takes an initialiser list") {
  WHEN("Given A Graph") {
    gdwg::Graph<char, std::string> b{'a', 'b', 'c',};
    THEN("testing the graph construct") {
      REQUIRE(b.IsNode('a'));
      REQUIRE(b.IsNode('b'));
      REQUIRE(b.IsNode('c'));
    }
  }
}

SCENARIO("copy constructor") {
  WHEN("Given 2 Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    gdwg::Graph<std::string, int> a{b};
    THEN("testing the graph construct") { REQUIRE(a == b); }
  }
}
SCENARIO("move constructor") {
  WHEN("Given 2 Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    gdwg::Graph<std::string, int> a{std::move(b)};
    THEN("testing the 2 graph construct") {
      REQUIRE(!(b.IsNode("a")));
      REQUIRE(!(b.IsNode("b")));
      REQUIRE(!(b.IsNode("c")));
      REQUIRE(a.IsNode("a"));
      REQUIRE(a.IsNode("b"));
      REQUIRE(a.IsNode("a"));
      REQUIRE(a.IsConnected("a", "b"));
      REQUIRE(a.IsConnected("b", "c"));
    }
  }
}
SCENARIO("copy assignment") {
  WHEN("Given 2 Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    gdwg::Graph<std::string, int> a = b;
    THEN("testing the 2 graph construct") { REQUIRE(a == b); }
  }
}
SCENARIO("move assignment") {
  WHEN("Given 2 Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    gdwg::Graph<std::string, int> a = std::move(b);
    THEN("testing the 2 graph construct") {
      REQUIRE(!(b.IsNode("a")));
      REQUIRE(!(b.IsNode("b")));
      REQUIRE(!(b.IsNode("c")));
      REQUIRE(a.IsNode("a"));
      REQUIRE(a.IsNode("b"));
      REQUIRE(a.IsNode("a"));
      REQUIRE(a.IsConnected("a", "b"));
      REQUIRE(a.IsConnected("b", "c"));
    }
  }
}
SCENARIO("InsertNode") {
  WHEN("Given A Graph") {
    gdwg::Graph<std::string, int> a;
    a.InsertNode("r");
    THEN("testing whether r node  exists ") { REQUIRE(a.IsNode("r")); }
  }
}
SCENARIO("InsertEdge") {
  GIVEN("Given A Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    REQUIRE(b.IsConnected("a", "b"));
    REQUIRE(b.IsConnected("b", "c"));
    WHEN("insert edge a--d ") {
      REQUIRE_THROWS_WITH(
        b.InsertEdge("a", "d", 1),
        "Cannot call Graph::InsertEdge when either src or dst node does not exist");
    }
  }
}
SCENARIO("DeleteNode") {
  WHEN("Given A Graph") {
    gdwg::Graph<std::string, int> a;
    a.InsertNode("r");
    a.InsertNode("a");
    a.DeleteNode("r");
    THEN("testing whether r node  exists ") { REQUIRE(!(a.IsNode("r"))); }
  }
}
SCENARIO("Replace") {
  GIVEN("A gdwg::Graph object") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    b.Replace("a", "e");
    REQUIRE(b.IsNode("e"));
    WHEN("replace the node which does not exist ") {
      REQUIRE_THROWS_WITH(
        b.Replace("f", "e"),
        "Cannot call Graph::Replace on a node that doesn't exist");
    }
  }
}
SCENARIO("MergeReplace") {
  GIVEN("Given A Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertNode("d");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("a", "c", 2);
    b.InsertEdge("a", "d", 3);
    b.MergeReplace("a", "b");
    REQUIRE(!(b.IsNode("a")));
    REQUIRE(b.IsNode("b"));
    REQUIRE(b.IsNode("c"));
    REQUIRE(b.IsNode("d"));
    REQUIRE(b.IsConnected("b", "b"));
    REQUIRE(b.IsConnected("b", "c"));
    REQUIRE(b.IsConnected("b", "d"));
    WHEN("mergereplace the node which does not exist ") {
      REQUIRE_THROWS_WITH(
        b.MergeReplace("a", "e"),
        "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
    }
  }
}
SCENARIO("Clear") {
  WHEN("Given A Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    THEN("clear all nodes in the graph") {
      b.Clear();
      REQUIRE(!(b.IsNode("a")));
      REQUIRE(!(b.IsNode("b")));
      REQUIRE(!(b.IsNode("c")));
    }
  }
}
SCENARIO("IsNode") {
  WHEN("Given A Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    THEN("judge whether node a exists") { REQUIRE(b.IsNode("a")); }
  }
}
SCENARIO("IsConnected") {
  GIVEN("Given A Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    REQUIRE(b.IsConnected("a", "b"));
    WHEN("src or dst node don't exist in the graph") {
      REQUIRE_THROWS_WITH(
        b.IsConnected("a", "f"),
        "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
    }
  }
}

SCENARIO("GetNodes") {
  WHEN("Given A Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    std::vector<std::string> vc = b.GetNodes();
    THEN("judging the vector returned is right  ") {
      auto iter = vc.begin();
      REQUIRE(*iter == "a");
      REQUIRE(*(++iter) == "b");
      REQUIRE(*(++iter) == "c");
    }
  }
}
SCENARIO("GetConnected") {
  GIVEN("Given A Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertNode("d");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    b.InsertEdge("b", "d", 2);
    std::vector<std::string> vc = b.GetConnected("b");
    auto iter = vc.begin();
    REQUIRE(*iter == "c");
    REQUIRE(*(++iter) == "d");
    WHEN("src  node don't exist in the graph") {
      REQUIRE_THROWS_WITH(
        b.GetConnected("e"),
        "Cannot call Graph::GetConnected if src doesn't exist in the graph");
    
  }
}
SCENARIO("GetWeights") {
  GIVEN("Given A Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertNode("d");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("a", "b", 2);
    b.InsertEdge("b", "c", 2);
    b.InsertEdge("b", "d", 2);
    std::vector<int> vc = b.GetWeights("a", "b");
    auto iter = vc.begin();
    REQUIRE(*iter == 1);
    REQUIRE(*(++iter) == 2);
    WHEN("src or dst node don't exist in the graph") {
      REQUIRE_THROWS_WITH(
        b.GetWeights("d", "e"),
        "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
    }
  }
}

SCENARIO("erase") {
  WHEN("Given A Graph") {
    gdwg::Graph<std::string, int> b;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    THEN("erase edge a--b") {
      b.erase("a", "b", 1);
      REQUIRE(!(b.IsConnected("a", "b")));
    }
  }
}

SCENARIO("Friends equal") {
  WHEN("Given 2 Graphs") {
    gdwg::Graph<std::string, int> b;
    gdwg::Graph<std::string, int> a;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    a.InsertNode("a");
    a.InsertNode("b");
    a.InsertNode("c");
    a.InsertEdge("a", "b", 1);
    a.InsertEdge("b", "c", 2);
    THEN("judge whether graph a and graph b is the same") { REQUIRE(a == b); }
  }
}
SCENARIO("Friends not equal") {
  WHEN("Given 2 Graphs") {
    gdwg::Graph<std::string, int> b;
    gdwg::Graph<std::string, int> a;
    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    a.InsertNode("a");
    a.InsertNode("b");
    a.InsertNode("c");
    a.InsertEdge("a", "b", 2);
    a.InsertEdge("b", "c", 2);
    THEN("judge whether graph a and graph b is the same") { REQUIRE(a != b); }
  }
}
SCENARIO("os") {
  WHEN("Given A Graph") {
    gdwg::Graph<std::string, int> b;
    std::stringstream os1;
    std::stringstream os2;

    b.InsertNode("a");
    b.InsertNode("b");
    b.InsertNode("c");
    b.InsertEdge("a", "b", 1);
    b.InsertEdge("b", "c", 2);
    b.InsertEdge("c", "a", 2);
    os2 << b;
    os1 << "a ("
        << "\n"
        << "  b | 1"
        << "\n"
        << ")\n"
        << "b ("
        << "\n"
        << "  c | 2"
        << "\n"
        << ")\n"
        << "c ("
        << "\n"
        << "  a | 2"
        << "\n"
        << ")\n";
    THEN("judge whether graph os1 and graph os2 is the same") { REQUIRE(os1.str() == os2.str()); }
  }
}