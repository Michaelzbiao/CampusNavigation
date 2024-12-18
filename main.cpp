#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include "LGraph.h"
#include "Algorithm.h"
#include "LocationInfo.h"


using Graph::LGraph;
using Graph::VertInfo;
using namespace Graph::Algorithm;

// Hint: 如果不慎写坏项目附带的两个文件，可以在backup文件夹中获取原始文件
// TODO: 请根据可执行文件和csv的位置填写文件路径
const std::string nodes_path{ "nodes1.csv" };
const std::string edges_path{ "edges1.csv" };

struct edge {
    std::string from;
    std::string to;
    int length;
};

void init(LGraph& graph);

std::vector<VertInfo> read_nodes(const std::string& nodes_path);

std::vector<edge> read_edges(const std::string& edges_path);

void store_nodes(const std::string& nodes_path, const LGraph& graph);

void store_edges(const std::string& edges_path, const LGraph& graph);

void show_all_nodes(const LGraph& graph);

void show_all_edges(const LGraph& graph);

int main() {
    LGraph graph(false); //初始化一个无向图。
    init(graph);
    while (true) {
        int choice{ 0 };
        std::cout << "欢迎使用校园导航系统！" << std::endl
            << "请选择您要进行的操作：" << std::endl
            << "1.顶点相关操作" << std::endl
            << "2.边相关操作" << std::endl
            << "3.从文件中重新加载点与边" << std::endl
            << "4.是否存在欧拉通路" << std::endl
            << "5.求任意两点间的最短距离" << std::endl
            << "6.求最小生成树" << std::endl
            << "7.求解拓扑受限时的最短路径" << std::endl
            << "8.图是否连通" << std::endl
			<< "9.多个目标地点的最短路径" << std::endl
            << "10.退出程序" << std::endl
            << "请输入操作前的数字：";
        std::cin >> choice;
        if (choice == 1) { // 顶点相关操作
            std::cout << "顶点相关操作：" << std::endl
                << "1.输出特定顶点信息" << std::endl
                << "2.输出所有顶点信息" << std::endl
                << "3.添加一个顶点" << std::endl
                << "4.删除一个顶点" << std::endl
				<< "5.更新一个顶点" << std::endl
                << "6.存储顶点到文件中" << std::endl
                << "7.按照类别查找地点" << std::endl
                << "8.回到上一级菜单" << std::endl
                << "请输入操作前的数字：";
            std::cin >> choice;
            if (choice == 1) {
                std::string name;
                std::cout << "请输入顶点名称：";
                std::cin >> name;
                try {
                    VertInfo v{ graph.GetVertex(name) };
                    std::cout << "顶点名称：" << v.name << std::endl
                        << "顶点类型：" << v.type << std::endl
                        << "建议游览时间：" << v.visitTime << std::endl;

                }
                catch (Graph::GraphException& e) {
                    std::cout << "名称为 " << name << " 的顶点不存在" << std::endl;
                }
            }
            else if (choice == 2) {
                std::cout << "顶点名称, 顶点类型, 建议游览时间：" << std::endl;
                show_all_nodes(graph);
            }
            else if (choice == 3) {
                std::cout << "请输入顶点名称，顶点类型，建议游览时间(空格分隔)：";
                std::string name, type;
                int visit_time;
                std::cin >> name >> type >> visit_time;
                try {
                    graph.InsertVertex({ name, visit_time, type });
                }
                catch (Graph::GraphException& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            else if (choice == 4) {
                std::cout << "请输入顶点名称：";
                std::string name;
                std::cin >> name;
                try {
                    graph.DeleteVertex({ name, 0, "" });
                }
                catch (Graph::GraphException& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            else if (choice == 5) {
				std::cout << "请输入顶点名称，新类型，新建议游览时间(空格分隔)：";
				std::string name, type;
				int new_visit_time;
				std::cin >> name >> type >> new_visit_time;
				try {
                    VertInfo new_info{ name,new_visit_time, type };
                    graph.UpdateVertex({ name, 0, "" }, new_info); // 假设 UpdateVertex 方法已实现
                    std::cout << "顶点信息更新成功！" << std::endl;
				}
				catch (Graph::GraphException& e) {
					std::cout << e.what() << std::endl;
				}

            }
            else if (choice == 6) {
                std::cout << "正在将顶点存储到test_nodes.csv中..." << std::endl;
                store_nodes("test_nodes.csv", graph);
                std::cout << "存储成功！" << std::endl;
            }
			else if (choice == 7) {
				std::cout << "请输入您要查找的地点类型：" << std::endl;
				std::string type;
				std::cin >> type;
                std::cout << "顶点名称, 顶点类型, 建议游览时间：" << std::endl;
				for (const auto& head_node : graph.List()) {
					if (graph.Deleted().find(head_node.data.name) != graph.Deleted().end())
                    {
						continue; // 如果已删除，则跳过
					}
					if (head_node.data.type == type) {
						std::cout << head_node.data.name << "," << head_node.data.type << "," << head_node.data.visitTime << std::endl;
					}
				}
			}
        }
        else if (choice == 2) { // 边相关操作
            std::cout << "边相关操作：" << std::endl
                << "1.输出特定边信息" << std::endl
                << "2.输出所有边信息" << std::endl
                << "3.添加一条边" << std::endl
                << "4.删除一条边" << std::endl
				<< "5.更新一条边" << std::endl
                << "6.存储边到文件中" << std::endl
                << "7.查找与顶点有关的道路" << std::endl
				<< "8.按权重降序输出所有边" << std::endl
                << "9.回到上一级菜单" << std::endl
                << "请输入操作前的数字：";
            std::cin >> choice;
            if (choice == 1) {
                std::string from, to;
                std::cout << "请输入边的起点与终点(空格分隔)：";
                std::cin >> from >> to;
                try {
                    Graph::GElemSet w{ graph.GetEdge(from, to) };
                    std::cout << from << " <---> " << to << " 距离为：" << w << std::endl;
                }
                catch (Graph::GraphException& e) {
                    std::cout << from << " <---> " << to << " 不存在！" << std::endl;
                }
            }
            else if (choice == 2) {
                std::cout << "起点, 终点, 距离：" << std::endl;
                show_all_edges(graph);
            }
            else if (choice == 3) {
                std::cout << "请输入起点名称，终点名称，距离(空格分隔)：";
                std::string from, to;
                int length;
                std::cin >> from >> to >> length;
                graph.InsertEdge(from, to, length);
            }
            else if (choice == 4) {
                std::cout << "请输入起点名称，终点名称(空格分隔)：";
                std::string from, to;
                std::cin >> from >> to;
                graph.DeleteEdge(from, to); // 删除一条边
			}
			else if (choice == 5) {
				std::cout << "请输入起点名称，终点名称，新距离(空格分隔)：";
				std::string from, to;
				int new_weight;
				std::cin >> from >> to >> new_weight;
                try {
                    graph.UpdateEdge(from, to, new_weight); // 假设 UpdateEdge 方法已实现
                    std::cout << "边的权值更新成功！" << std::endl;
                }
                catch (Graph::GraphException& e) {
                    std::cout << e.what() << std::endl;
                }
			}
            else if (choice == 6) {
                std::cout << "正在将边存储到test_edges.csv文件中..." << std::endl;
                store_edges("test_edges.csv", graph);
                std::cout << "存储成功！" << std::endl;
            }
            else if (choice == 7) {
                std::cout << "请输入一个顶点，查找与之相关的所有道路：" << std::endl;
				std::string vertex;
				std::cin >> vertex;
				const auto& graph_list{ graph.List() };
				const auto& deleted_set{ graph.Deleted() };
                std::cout << "node,node,weight" << std::endl;
                for (const auto& head_node : graph_list) {
					if (head_node.data.name == vertex) {
                        if (deleted_set.find(head_node.data.name) != deleted_set.end()) {
							continue; // 如果已删除，则跳过
						}
                        for (const auto& edge : head_node.adj) {
							std::cout << vertex << "," << graph_list[edge.dest].data.name << "," << edge.weight << std::endl;
						}
						break;
					}
					for (const auto& edge : head_node.adj) {
                        if (graph_list[edge.dest].data.name == vertex) {
                            if (deleted_set.find(head_node.data.name) != deleted_set.end()) {
								continue; // 如果已删除，则跳过
							}
							std::cout << head_node.data.name << "," << vertex << "," << edge.weight << std::endl;
						}
					}
                }

				
            }
			else if (choice == 8) {
				std::cout << "按权重降序输出所有边：" << std::endl;
                std::cout << "node,node,weight" << std::endl;
                const auto& graph_edges = graph.SortedEdges(std::greater<>()); // 获取按降序排列的边
                const auto& graph_list = graph.List();

                for (const auto& en : graph_edges) {
                    const auto& e = edge{ graph_list[en.from].data.name, graph_list[en.dest].data.name, en.weight };
                    std::cout << e.from << "," << e.to << "," << e.length << std::endl;
                }
			}

        }
        else if (choice == 3) {// 从文件中重新加载点与边
            init(graph);
        }
        else if (choice == 4) { // 是否存在欧拉通路
            auto res = HaveEulerCircle(graph);
            std::cout << (res ? "存在欧拉回路" : "不存在欧拉回路") << std::endl;
        }
        else if (choice == 5) { // 求任意两点间的最短距离
            std::cout << "请输入两个地点，使用空格分开：" << std::endl;
            std::string x, y;
            std::cin >> x >> y;
            try {
                int shortestDistance = GetShortestPath(graph, x, y); // 调用 GetShortestPath 函数
                // 这里的 GetShortestPath 函数已经在内部处理了路径的输出
                if (shortestDistance != -1) {
                    std::cout << x << " 和 " << y << " 之间的最短距离为: " << shortestDistance << std::endl;
                }

            }
            catch (Graph::GraphException& e) {
                std::cout << "你找到了虚空的距离" << std::endl;
            }
        }
        else if (choice == 6) { // 求最小生成树
            if (IsConnected(graph)) {
                std::cout << "最小生成树的点如下："<<std::endl;
                auto res = MinimumSpanningTree(graph);
                int sum = 0;
                for (auto item : res) {
                    const auto& e = edge{ graph.List()[item.from].data.name, graph.List()[item.dest].data.name, item.weight };
                    std::cout << e.from << "," << e.to << "," << e.length << std::endl;
                    sum += e.length;
                }
                std::cout << "总权重为" << sum << std::endl;
            }
            else {
                std::cout << "图不连通" << std::endl;
            }
        }
        else if (choice == 7) { // 求解拓扑受限时的最短路径
            std::cout << "请输入您希望的拓扑序，第一行一个n为序列长度，第二行n个地点为拓扑序列" << std::endl;
            int n;
            std::vector<std::string> list;
            std::cin >> n;
            while (n--) {
                std::string x;
                std::cin >> x;
                list.push_back(x);
            }
            std::cout << "最短路径为" << TopologicalShortestPath(graph, list) << std::endl;
        }
		else if (choice == 8) { // 判断图是否连通
			std::cout << (IsConnected(graph) ? "图是连通的" : "图不连通") << std::endl;
		}
		else if (choice == 9) { // 多个目标地点的最短路径
            std::cout << "请输入多个目标地点的访问顺序，使用空格分开：" << std::endl;
            std::vector<std::string> orderedTargets;
            std::string target;

            while (std::cin >> target) {
                orderedTargets.push_back(target);
                if (std::cin.peek() == '\n') break; // 读取到换行符停止输入
            }

            try {
                int totalDistance = Graph::Algorithm::FindShortestPathWithOrder(graph, orderedTargets);
            }
            catch (Graph::GraphException& e) {
                std::cout << e.what() << std::endl; // 输出异常信息
            }
        }
        else {
            std::cout << "感谢您的使用，再见！\n 开发者：\\lky1433223/ \\Voltline/" << std::endl;
            break;
        }
    }
    return 0;
}

/* 从文件中读取顶点信息
 * @param nodes_path: 顶点文件路径
 * @return: 包含读取到的所有顶点的std::vector
 */
std::vector<VertInfo> read_nodes(const std::string& nodes_path) {
    std::ifstream fin(nodes_path);              // 创建文件输入流对象
    assert(fin.good());                         // 断言，通过文件输入流的good()方法检查文件是否存在
    std::vector<VertInfo> nodes;                // 临时存储顶点信息
    std::string line;                           // 通过std::getline读取到的每一行，临时存储在字符串line中
    while (std::getline(fin, line)) {       // 通过std::getline(std::istream&, std::string&)来获取输入流的一行
        for (auto& c : line) {
            if (c == ',') c = ' ';              // csv文件以逗号分隔，为了方便后续使用字符串输入流，将逗号替换为空格
        }
        std::istringstream sin(line);           // 利用std::istringstream，以获取的行为内容构建一个字符串输入流
        std::string name, type;                 // 文件每行中的顶点名，顶点类型
        int visit_time;                         // 文件中每行的顶点推荐游览时间
        sin >> name >> type >> visit_time;      // 利用已构造的字符串输入流，获取对应的几个参数
        nodes.push_back({ name, visit_time, type });  // 放入需要返回的std::vector中
    }
    fin.close();                                // 文件使用完毕后，请务必使用close访问关闭输入流，避免文件损坏
    return nodes;
}

/* 从文件中读取边信息
 * @param edges_path: 边文件路径
 * @return: 包含读取到的所有边的std::vector
 */
std::vector<edge> read_edges(const std::string& edges_path) {
    // TODO: 请参考read_nodes函数的内容与提示，结合edges.csv文件的内容，完成本函数的定义
    std::ifstream fin(edges_path);
    assert(fin.good());
    std::vector<edge> edges;
    std::string line;
    while (std::getline(fin, line)) {
        for (auto& c : line) {
            if (c == ',')c = ' ';
        }
        std::istringstream sin(line);
        std::string from, to;
        int length;
        sin >> from >> to >> length;
        edges.push_back({ from,to,length });
    }
    return edges;
}

void init(LGraph& graph) {
	graph.Clear();                                                // 清空图
    std::vector<VertInfo> nodes{ read_nodes(nodes_path) };        // 通过read_nodes函数获得顶点信息
    std::vector<edge> edges{ read_edges(edges_path) };            // 通过read_edges函数获得边信息
    for (auto& v : nodes) {
        // TODO: 向图graph中插入顶点
		graph.InsertVertex(v);
    }
    for (auto& e : edges) {
        // TODO: 向图graph中插入边
		graph.InsertEdge(e.from, e.to, e.length);

    }
}

/* 打印目前的所有顶点
 * @param graph: 图
 */
void show_all_nodes(const LGraph& graph) {
    // TODO: 实现打印所有顶点信息
    // TODO: 每行输出格式：顶点名,顶点类型,推荐游览时间
	const auto& graph_list{ graph.List() };//获取邻接表
    const auto& deleted_set{ graph.Deleted()}; // 获取已删除的顶点集合
    for (const auto& head_node : graph_list) {
        // 获取顶点信息
        if (deleted_set.find(head_node.data.name) != deleted_set.end()) {
            continue; // 如果已删除，则跳过
        }
        const auto& vertex_info = head_node.data;

        // 输出顶点名称、类型、推荐游览时间
        std::cout << vertex_info.name << "," << vertex_info.type << "," << vertex_info.visitTime << std::endl;
    }
  

}

/* 打印目前的所有边
 * @param graph: 图
 */
void show_all_edges(const LGraph& graph) {
    // LGraph类只提供了获取邻接表和获取经过排序的边的方法
    // 为了能够获取到每条边的元信息，我们需要配合两组数据来完成信息的获取
    const auto& graph_edges{ graph.SortedEdges() };       // 获取经过排序的边
    const auto& graph_list{ graph.List() };               // 获取邻接表
    for (const auto& en : graph_edges) {                 // 遍历边，通过下标来获取对应的顶点信息
        const auto& e = edge{ graph_list[en.from].data.name, graph_list[en.dest].data.name, en.weight };
        std::cout << e.from << "," << e.to << "," << e.length << std::endl;
    }
}

/* 将顶点信息存储回文件中
 * @param nodes_path: 顶点文件路径
 * @param graph: 图
 */
void store_nodes(const std::string& nodes_path, const LGraph& graph) {
    std::ofstream fout(nodes_path);                                         // 通过目录创建文件输出流
    const auto& graph_list{ graph.List() };                                   // 通过LGraph对象的List()方法获得邻接表
    const auto& deleted_set{ graph.Deleted()}; // 获取已删除的顶点集合
    for (const auto& n : graph_list) {                                       // 遍历邻接表中的每个顶点
        if (deleted_set.find(n.data.name) != deleted_set.end()) {
            continue; // 如果已删除，则跳过
        }
        const auto& v = n.data;                                             // 获得顶点中的元数据
        fout << v.name << "," << v.type << "," << v.visitTime << std::endl; // 按照顶点存储的格式，重新输入到文件中
    }
    fout.close();                                                           // 在使用完任何文件流后，都要使用close方法关闭
}

/* 将边信息存储回文件中
 * @param edges_path: 边文件路径
 * @param graph: 图
 */
void store_edges(const std::string& edges_path, const LGraph& graph) {
    // TODO: 请参考store_nodes函数的内容与提示，结合edges.csv文件的内容
    // TODO: 以及show_all_edges函数中获取边的元信息的方式完成store_edges函数的定义
	std::ofstream fout(edges_path);
	const auto& graph_edges{ graph.SortedEdges() };
	const auto& graph_list{ graph.List() };
	for (const auto& en : graph_edges) {
		const auto& e = edge{ graph_list[en.from].data.name, graph_list[en.dest].data.name, en.weight };
		fout << e.from << "," << e.to << "," << e.length << std::endl;
	}
	fout.close();

}