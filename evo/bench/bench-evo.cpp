#include <bench/benc.h>

#include <core/random.h>
#include <evo/evo.h>

#include <iostream>

void KeepOnlyFirstAgents(const size_t numToKeep, evo::Generation& gen) {
    evo::KeepOnlyFirstAgents(numToKeep, &gen);
}

void SortGenerationByScores(evo::Generation& gen) { evo::SortGenerationByScores(&gen); }

int main() {
    DECLARE_BENCHMARK_SET(evoBench);

    // ********** GetNumChildren ********** //
    RUN_BENCH(evoBench, evo::GetNumChildren, "basic", {
        thread_local RandRealDistribution rand(0.0, 100.0);
        const double minVal = rand.get();
        const double ourVal = minVal + rand.get();
        const double maxVal = ourVal + rand.get();
        return std::make_tuple(ourVal, minVal, maxVal);
    });

    RUN_BENCH(evoBench, KeepOnlyFirstAgents, "nontrivial", {
        thread_local RandIntDistribution rand(1, 50);
        const size_t numToKeep = rand.get();
        evo::Generation gen;
        for (size_t i = 0; i < numToKeep + rand.get(); i++) {
            gen.agents.push_back(evo::Agent());
        }

        return std::make_tuple(numToKeep, gen);
    });

    // ********** KeepOnlyFirstAgentsw ********** //
    RUN_BENCH(evoBench, KeepOnlyFirstAgents, "trivial", {
        thread_local RandIntDistribution rand(1, 50);
        evo::Generation gen;
        for (size_t i = 0; i < rand.get(); i++) {
            gen.agents.push_back(evo::Agent());
        }
        const size_t numToKeep = gen.agents.size() + rand.get();

        return std::make_tuple(numToKeep, gen);
    });

    // ********** SortGenerationByScores ********** //
    RUN_BENCH(evoBench, SortGenerationByScores, "10", {
        thread_local RandRealDistribution rand(0.001, 150.0);
        evo::Generation gen;
        for (size_t i = 0; i < 10; i++) {
            evo::Agent agent;
            agent.score = rand.get();
            gen.agents.push_back(agent);
        }

        return std::make_tuple(gen);
    });

    RUN_BENCH(evoBench, SortGenerationByScores, "50", {
        thread_local RandRealDistribution rand(0.001, 150.0);
        evo::Generation gen;
        for (size_t i = 0; i < 50; i++) {
            evo::Agent agent;
            agent.score = rand.get();
            gen.agents.push_back(agent);
        }

        return std::make_tuple(gen);
    });

    RUN_BENCH(evoBench, SortGenerationByScores, "100", {
        thread_local RandRealDistribution rand(0.001, 150.0);
        evo::Generation gen;
        for (size_t i = 0; i < 100; i++) {
            evo::Agent agent;
            agent.score = rand.get();
            gen.agents.push_back(agent);
        }

        return std::make_tuple(gen);
    });

    END_BENCHMARK_SET(evoBench);

    return 0;
}