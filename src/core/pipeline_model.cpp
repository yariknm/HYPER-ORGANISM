#include "hyper_organism/core/types.hpp"
#include "hyper_organism/isa/backend.hpp"

#include <cstring>
#include <random>
#include <algorithm>

namespace hyper_organism {

void DependencyGraph::build() {
    edges.resize(nodes.size());
    topological_order.clear();
    
    // Build dependency edges based on register usage
    std::vector<int> last_write(256, -1); // track last write to each register
    
    for (size_t i = 0; i < nodes.size(); ++i) {
        auto& inst = nodes[i];
        inst.dependencies.clear();
        inst.dependents.clear();
        
        // Add edges from previous writes to same registers
        for (int prev : last_write) {
            if (prev >= 0 && prev < static_cast<int>(i)) {
                edges[prev].push_back(static_cast<uint32_t>(i));
                inst.dependencies.push_back(static_cast<uint32_t>(prev));
                nodes[prev].dependents.push_back(static_cast<uint32_t>(i));
            }
        }
        
        // Update last write
        if (inst.reg < 256) {
            last_write[inst.reg] = static_cast<int>(i);
        }
    }
    
    // Simple topological sort
    std::vector<bool> visited(nodes.size(), false);
    std::vector<uint32_t> order;
    
    std::function<void(uint32_t)> dfs = [&](uint32_t node) {
        visited[node] = true;
        for (uint32_t dep : edges[node]) {
            if (!visited[dep]) {
                dfs(dep);
            }
        }
        order.push_back(node);
    };
    
    for (uint32_t i = 0; i < nodes.size(); ++i) {
        if (!visited[i]) {
            dfs(i);
        }
    }
    
    std::reverse(order.begin(), order.end());
    topological_order = order;
}

void DependencyGraph::compute_critical_path() {
    if (nodes.empty()) {
        critical_path_ = 0;
        return;
    }
    
    std::vector<size_t> dp(nodes.size(), 0);
    size_t max_path = 0;
    
    for (uint32_t idx : topological_order) {
        size_t max_pred = 0;
        for (uint32_t dep : nodes[idx].dependencies) {
            max_pred = std::max(max_pred, dp[dep]);
        }
        dp[idx] = max_pred + nodes[idx].pipeline_latency;
        max_path = std::max(max_path, dp[idx]);
    }
    
    critical_path_ = max_path;
}

} // namespace hyper_organism