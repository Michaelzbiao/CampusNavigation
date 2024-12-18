//
// Created by 刘凯源 on 24-10-13.
//
#include "Algorithm.h"
#include <queue>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <functional>
#include <numeric>
namespace Graph {
    namespace Algorithm {
        std::list<Vertex> GetCircuit(LGraph& graph, Vertex start) {
            // 从给定点出发获得一条回路
            std::vector<bool> visited(graph.VertexCount(), false);
            std::list<Vertex> circuit; // Change to std::list
            std::stack<Vertex> stack;
            stack.push(start);
            visited[start] = true;

            while (!stack.empty()) {
                Vertex current = stack.top();
                stack.pop();
                circuit.push_back(current); // Add to list instead of vector

                for (const auto& edge : graph.List()[current].adj) {
                    if (!visited[edge.dest]) {
                        visited[edge.dest] = true;
                        stack.push(edge.dest);
                    }
                }
            }

            return circuit; // Return the list of vertices
        }

        std::list<Vertex> EulerCircle(LGraph& graph) {
            // 获取欧拉回路,你可以使用GetCircuit函数
            if (!HaveEulerCircle(graph)) {
                return {};
            }

            LGraph temp_graph = graph;
            return GetCircuit(temp_graph, 0); // 从第一个顶点开始
        }

        bool HaveEulerCircle(LGraph& graph) {
            if (!IsConnected(graph)) {
                return false;
            }

            for (const auto& head_node : graph.List()) {
                if (head_node.adj.size() % 2 != 0) {
                    return false; // 如果某个节点的度数为奇数，则不存在欧拉回路
                }
            }

            return true; // 所有节点的度数为偶数，且图是连通的
        }

        void BFSv(const LGraph& graph, Vertex v, std::vector<bool>& visited) {
            // 广度优先搜索整个图
            std::queue<Vertex> queue;
            visited[v] = true;
            queue.push(v);

            while (!queue.empty()) {
                Vertex u = queue.front();
                queue.pop();

                for (const auto& edge : graph.List()[u].adj) { // 遍历u的邻接点
                    if (!visited[edge.dest]) {
                        visited[edge.dest] = true;
                        queue.push(edge.dest);
                    }
                }
            }
        }

        bool IsConnected(const LGraph& graph) {
            // 判断图是否联通
            std::vector<bool> visited(graph.VertexCount(), false);
            BFSv(graph, 0, visited); // 从第一个点开始遍历

            for (bool v : visited) {
                if (!v) {
                    return false; // 如果有未访问的节点，则图不连通
                }
            }

            return true; // 所有节点都被访问过，图是连通的
        }

        int GetShortestPath(const LGraph& graph, const std::string& vertex_name_x, const std::string& vertex_name_y) {
            // 获取两点之间的最短路径
            if (!graph.exist_vertex(vertex_name_x) || !graph.exist_vertex(vertex_name_y)) {
                throw Graph::GraphException("One or both vertices do not exist.");
            }

            Vertex start = graph.Map().at(vertex_name_x);
            Vertex end = graph.Map().at(vertex_name_y);

            std::vector<int> dist(graph.VertexCount(), std::numeric_limits<int>::max());
            std::vector<bool> visited(graph.VertexCount(), false);
            std::vector<Vertex> prev(graph.VertexCount(), -1); // 存储前驱节点
            dist[start] = 0;

            std::priority_queue<std::pair<int, Vertex>, std::vector<std::pair<int, Vertex>>, std::greater<>> pq;
            pq.push({ 0, start });

            while (!pq.empty()) {
                Vertex u = pq.top().second;
                pq.pop();

                if (visited[u]) continue;
                visited[u] = true;

                for (const auto& edge : graph.List()[u].adj) {
                    Vertex v = edge.dest;
                    int weight = edge.weight;

                    if (dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                        prev[v] = u; // 更新前驱节点
                        pq.push({ dist[v], v });
                    }
                }
            }

            // 输出最短路径
            if (dist[end] == std::numeric_limits<int>::max()) {
                return -1; // 返回 -1 表示没有路径
            }

            // 回溯构建路径
            std::vector<Vertex> path;
            for (Vertex at = end; at != -1; at = prev[at]) {
                path.push_back(at);
            }
            std::reverse(path.begin(), path.end()); // 反转路径

            // 输出路径
            std::cout << "从 " << vertex_name_x << " 到 " << vertex_name_y << " 的最短路径为: ";
            for (size_t i = 0; i < path.size(); ++i) {
                std::cout << graph.List()[path[i]].data.name; // 输出顶点名称
                if (i < path.size() - 1) {
                    std::cout << " -> "; // 输出箭头
                }
            }
			std::cout << std::endl;
            return dist[end]; // 返回最短路径长度
        }

        int TopologicalShortestPath(const LGraph& graph, std::vector<std::string> path) {
            // 获取拓扑受限的最短路径，拓扑序由path给出
            std::unordered_map<std::string, int> topo_index;
            for (int i = 0; i < path.size(); ++i) {
                topo_index[path[i]] = i;
            }

            std::vector<int> dist(path.size(), std::numeric_limits<int>::max());
            dist[0] = 0;

            for (int i = 0; i < path.size(); ++i) {
                Vertex u = graph.Map().at(path[i]);
                for (const auto& edge : graph.List()[u].adj) {
                    Vertex v = edge.dest;
                    int weight = edge.weight;
                    int v_index = topo_index[graph.List()[v].data.name];

                    if (dist[i] != std::numeric_limits<int>::max() && dist[i] + weight < dist[v_index]) {
                        dist[v_index] = dist[i] + weight;
                    }
                }
            }

            return dist.back() == std::numeric_limits<int>::max() ? -1 : dist.back(); // 返回拓扑序列的最短路径
        }

        std::vector<EdgeNode> MinimumSpanningTree(const LGraph& graph) {
            // 计算最小生成树，并返回树上的边
            std::vector<EdgeNode> result;
            std::vector<EdgeNode> edges = graph.SortedEdges();

            std::vector<int> parent(graph.VertexCount());
            std::iota(parent.begin(), parent.end(), 0);

            std::function<int(int)> find = [&](int u) {
                return parent[u] == u ? u : parent[u] = find(parent[u]);
                };

            for (const auto& edge : edges) {
                size_t u = find(edge.from);
                size_t v = find(edge.dest);

                if (u != v) {
                    result.push_back(edge);
                    parent[u] = v; // 合并两个集合
                }
            }

            return result; // 返回最小生成树的边
        }
        int GetShortestPath2(const LGraph& graph, const std::string& vertex_name_x, const std::string& vertex_name_y, std::vector<Vertex>& path) {
            // 获取两点之间的最短路径
            if (!graph.exist_vertex(vertex_name_x) || !graph.exist_vertex(vertex_name_y)) {
                throw Graph::GraphException("One or both vertices do not exist.");
            }

            Vertex start = graph.Map().at(vertex_name_x);
            Vertex end = graph.Map().at(vertex_name_y);

            std::vector<int> dist(graph.VertexCount(), std::numeric_limits<int>::max());
            std::vector<bool> visited(graph.VertexCount(), false);
            std::vector<Vertex> prev(graph.VertexCount(), -1); // 存储前驱节点
            dist[start] = 0;

            std::priority_queue<std::pair<int, Vertex>, std::vector<std::pair<int, Vertex>>, std::greater<>> pq;
            pq.push({ 0, start });

            while (!pq.empty()) {
                Vertex u = pq.top().second;
                pq.pop();

                if (visited[u]) continue;
                visited[u] = true;

                for (const auto& edge : graph.List()[u].adj) {
                    Vertex v = edge.dest;
                    int weight = edge.weight;

                    if (dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                        prev[v] = u; // 更新前驱节点
                        pq.push({ dist[v], v });
                    }
                }
            }

            // 输出最短路径
            if (dist[end] == std::numeric_limits<int>::max()) {
                return -1; // 返回 -1 表示没有路径
            }

            // 回溯构建路径
            path.clear();
            for (Vertex at = end; at != -1; at = prev[at]) {
                path.push_back(at);
            }
            std::reverse(path.begin(), path.end()); // 反转路径

            return dist[end]; // 返回最短路径长度
        }
        int FindShortestPathWithOrder(const LGraph& graph, const std::vector<std::string>& orderedTargets) {
            // 检查目标地点是否存在
            for (const auto& target : orderedTargets) {
                if (!graph.exist_vertex(target)) {
                    throw Graph::GraphException("目标地点不存在: " + target);
                }
            }

            // 计算总路径长度
            int totalDistance = 0;
            std::vector<Vertex> path;

            for (size_t i = 0; i < orderedTargets.size() - 1; ++i) {
                std::vector<Vertex> subPath;
                int distance = GetShortestPath2(graph, orderedTargets[i], orderedTargets[i + 1], subPath);

                if (distance == -1) {
                    throw Graph::GraphException("无法从 " + orderedTargets[i] + " 到 " + orderedTargets[i + 1] + " 的路径.");
                }

                totalDistance += distance;
                path.insert(path.end(), subPath.begin(), subPath.end()); // 添加子路径
            }

            // 输出路径
            std::cout << "访问顺序为: ";
            for (const auto& target : orderedTargets) {
                std::cout << target << " ";
            }
            std::cout << std::endl;

            std::cout << "总路径长度为: " << totalDistance <<  std::endl;

            return totalDistance;
        }
    }
    
}