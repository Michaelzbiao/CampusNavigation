//
// Created by 刘凯源 on 24-10-13.
//

#ifndef CAMPUSNAVIGATION_ALGORITHM_H
#define CAMPUSNAVIGATION_ALGORITHM_H

#include "LGraph.h"


namespace Graph {
    namespace Algorithm {
        class DSU {
        private:
			std::vector<int> parent, rank;//存储每个节点的父节点和秩
        public:
            // 并查集构造函数
            explicit DSU(int n) {
                parent.resize(n);
                rank.resize(n, 0);
                for (int i = 0; i < n; ++i) {
                    parent[i] = i; // 初始化每个节点的父节点为自己
                }
                // TODO: 请完成并查集的构造函数
            }

            // 查找根节点并路径压缩
            int find(int x) {
                if (parent[x] != x) {
                    parent[x] = find(parent[x]); // 路径压缩
                }
                return parent[x];
            }

            // const 版本的查找
            int find(int x) const {
                return find(x); // 调用非 const 版本
            }

            // 合并两个集合
            void unite(int x, int y) {
                // TODO: 合并集合
                int rootX = find(x);
                int rootY = find(y);
                if (rootX != rootY) {
                    // 按秩合并
                    if (rank[rootX] < rank[rootY]) {
                        parent[rootX] = rootY; // 将较小秩的树挂到较大秩的树下
                    }
                    else if (rank[rootX] > rank[rootY]) {
                        parent[rootY] = rootX;
                    }
                    else {
                        parent[rootY] = rootX; // 如果秩相同，任意选择一个作为根
                        rank[rootX]++; // 增加新根的秩
                    }
                }

            }

            // 检查两个节点是否属于同一集合
            bool same(int x, int y) {
                // TODO: 检查两个节点是否属于同一个集合
                return find(x) == find(y);
            }
        };

        std::list<Vertex> GetCircuit(LGraph& graph, Vertex start); // 从给定点出发获得一条回路
        std::list<Vertex> EulerCircle(LGraph& graph); // 获取欧拉回路
        bool HaveEulerCircle(LGraph& graph); // 判断是否存在欧拉回路
        bool IsConnected(const LGraph& graph); // 判断图是否联通
        int GetShortestPath(const LGraph& graph, const std::string& vertex_name_x, const std::string& vertex_name_y); // 计算单源最短路径
        int TopologicalShortestPath(const LGraph& graph, std::vector<std::string> path); // 计算拓扑受限的最短路径
        std::vector<EdgeNode> MinimumSpanningTree(const LGraph& graph); // 计算最小生成树
		int FindShortestPathWithOrder(const LGraph& graph, const std::vector<std::string>& orderedTargets); // 计算有序路径的最短路径
		int GetShortestPath2(const LGraph& graph, const std::string& vertex_name_x, const std::string& vertex_name_y, std::vector<Vertex>& path); // 计算单源最短路径
    }
}
#endif //CAMPUSNAVIGATION_ALGORITHM_H
