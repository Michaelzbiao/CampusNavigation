//
// Created by 刘凯源 on 24-5-16.
//

#include "LGraph.h"
namespace Graph {
    LGraph::LGraph(bool directed)
        : n_verts(0), m_edges(0), directed(directed), ver_list(std::vector<HeadNode>()) {}

    bool LGraph::exist_vertex(const std::string& name) const {
        //TODO:判断是否存在name名称的节点
        auto it = vertex_map.find(name);
        if (it == vertex_map.end())
        {
			return false;
        }
        return vertex_map.find(name) != vertex_map.end();

    }
    void LGraph::Clear() {
        ver_list.clear(); // 清空邻接表
        vertex_map.clear(); // 清空顶点映射
        deleted.clear(); // 清空已删除的节点集合
        n_verts = 0; // 重置顶点计数
        m_edges = 0; // 重置边计数
    }

    bool LGraph::exist_edge(const std::string& vertex_x_name, const std::string& vertex_y_name) const {
        //TODO:判断是否存在x-y的边
        auto it_x = vertex_map.find(vertex_x_name);
        auto it_y = vertex_map.find(vertex_y_name);
        if (it_x == vertex_map.end() || it_y == vertex_map.end()) return false;//检查点存不存在

        Vertex x = it_x->second;//找到地点的ID映射
        Vertex y = it_y->second;

        for (const auto& edge : ver_list[x].adj) {//x为起点，y为终点存在
            if (edge.dest == y) return true;
        }
        return false;

    }

    void LGraph::InsertVertex(const LocationInfo& vertex_info) {
        //TODO:插入节点，节点信息由LocationInfo类给出
        if (exist_vertex(vertex_info.name)) {
			throw GraphException("顶点已经存在了");
        }
        Vertex new_vertex = ver_list.size();//得到ver_list已经存了几个顶点，从而得到当前的地点的ID映射
        ver_list.emplace_back(vertex_info);
        /*根据HeadNode中explicit HeadNode(const VertInfo& data_)
            : data(data_), adj(std::list<EdgeNode>()) {}
           即使插入的不是HeadNode,是LocationInfo，会自动转换 */
        vertex_map[vertex_info.name] = new_vertex;
        n_verts++;
       
    }

    void LGraph::DeleteVertex(const LocationInfo& vertex_info) {
        


        //TODO:删除节点，节点信息由LocationInfo类给出
        auto it = vertex_map.find(vertex_info.name);
        if (it == vertex_map.end())//检查顶点是否存在
        {
			throw GraphException("顶点不存在");
        }
        //删除ver_list中的顶点，ver_list[vertex]的data不变，后序加节点，的映射还是继续加1
        Vertex vertex = it->second;
        ver_list[vertex].adj.clear();
        deleted.insert(vertex_info.name);
        //删除其他结点指向指定结点的边
        for (auto& head_node : ver_list) {
            head_node.adj.remove_if([vertex](const EdgeNode& edge) {
                return edge.dest == vertex;
                });
        }
        vertex_map.erase(it);
        n_verts--;
       
    }

    void LGraph::UpdateVertex(const LocationInfo& old_info, LocationInfo& new_info) {
        //TODO:更新节点，新/旧节点的信息由LocationInfo类给出
        auto it = vertex_map.find(old_info.name);
        if (it == vertex_map.end())
        {
			throw GraphException("顶点不存在");
        }

        Vertex vertex = it->second;
        ver_list[vertex].data = new_info;
        vertex_map.erase(it);
        vertex_map[new_info.name] = vertex;
       
    }

    VertInfo LGraph::GetVertex(const std::string& name) const {
        // Find the vertex in the vertex_map
        auto it = vertex_map.find(name);
        if (it == vertex_map.end()) {
			throw GraphException("Vertex does not exist.");
        }
        // Retrieve and return the vertex information
        VertInfo temp = ver_list[it->second].data;
        return temp;
    }

    VertInfo LGraph::GetVertex(const Vertex vertex) const {
        // Check if the vertex ID is within bounds
        if (vertex >= ver_list.size()) {
			throw GraphException("顶点不存在");
        }
        // Retrieve and return the vertex information
        return ver_list[vertex].data;
    }

    void LGraph::InsertEdge(const std::string& vertex_x_name, const std::string& vertex_y_name, GElemSet weight) {
        //TODO:插入边
        if (exist_edge(vertex_x_name, vertex_y_name)) return;
        Vertex x = vertex_map[vertex_x_name];
        Vertex y = vertex_map[vertex_y_name];
        ver_list[x].adj.emplace_back(x, y, weight);

        if (!directed) {//如果无方向，则从x到y，还要从y到x
            ver_list[y].adj.emplace_back(y, x, weight);
        }
        m_edges++;
        

    }

    void LGraph::DeleteEdge(const std::string& vertex_x_name, const std::string& vertex_y_name) {
        //TODO:删除边，由两个节点名确定一条边
        Vertex x = vertex_map[vertex_x_name];
        Vertex y = vertex_map[vertex_y_name];

        ver_list[x].adj.remove_if([y](const EdgeNode& edge) {
            return edge.dest == y;
            });

        if (!directed) {
            ver_list[y].adj.remove_if([x](const EdgeNode& edge) {
                return edge.dest == x;
                });
        }
        m_edges--;
        

    }

    void LGraph::DeleteEdge(Vertex vertex_x, Vertex vertex_y) {
        //TODO:删除边，由两个节点ID确定一条边
        Vertex x = vertex_x;
        Vertex y = vertex_y;
        ver_list[x].adj.remove_if([y](const EdgeNode& edge) {
            return edge.dest == y;
            });

        if (!directed) {
            ver_list[y].adj.remove_if([x](const EdgeNode& edge) {
                return edge.dest == x;
                });
        }
        m_edges--;
       
    }


    void LGraph::UpdateEdge(const std::string& vertex_x_name, const std::string& vertex_y_name, GElemSet new_weight) {
        //TODO:更新边，由两个节点名确定一条边
        Vertex x = vertex_map[vertex_x_name];
        Vertex y = vertex_map[vertex_y_name];

        for (auto& edge : ver_list[x].adj) {
            if (edge.dest == y) {
                edge.weight = new_weight;
                break;
            }
        }

        if (!directed) {
            for (auto& edge : ver_list[y].adj) {
                if (edge.dest == x) {
                    edge.weight = new_weight;
                    break;
                }
            }
        }
        
    }

    GElemSet LGraph::GetEdge(const std::string& vertex_x_name, const std::string& vertex_y_name) const {
        // Find the vertices in the vertex_map
        auto it1 = vertex_map.find(vertex_x_name);
        auto it2 = vertex_map.find(vertex_y_name);
        if (it1 == vertex_map.end() || it2 == vertex_map.end()) {
			throw GraphException("边不存在");
        }
        // Search for the edge from vertex_x to vertex_y
        for (const auto& edge : ver_list[it1->second].adj) {
            if (edge.dest == it2->second) {
                return edge.weight; // Return the weight of the edge if found
            }
        }
        return GElemSet(); // If the edge is not found, return a default GElemSet object
    }

    std::vector<EdgeNode> LGraph::SortedEdges(std::function<bool(const GElemSet&, const GElemSet&)> cmp) const {
        std::vector<EdgeNode> edges;

        // Gather all edges into the edges vector
        for (const auto& head : ver_list) {
            for (const auto& edge : head.adj) {
                if (directed || edge.from < edge.dest) {
                    edges.push_back(edge);
                }
            }
        }

        // Insertion sort: Sort edges based on the provided comparator
        for (size_t i = 1; i < edges.size(); ++i) {
            EdgeNode key = edges[i];  // The edge to be inserted
            size_t j = i;

            // Find the correct position for key
            while (j > 0 && cmp(key.weight, edges[j - 1].weight)) {
                edges[j] = edges[j - 1];  // Move elements greater than key one position ahead
                --j;
            }

            // Insert key at the correct position
            edges[j] = key;
        }

        return edges;
    }


}