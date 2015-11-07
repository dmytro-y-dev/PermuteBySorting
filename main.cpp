#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <utility>
#include <functional>
#include <cassert>
#include <sstream>
#include <chrono>

using namespace std;

template <class value_type>
void PermuteBySortingUnique(vector<value_type>& A)
{
  int n = A.size();

  vector<int> P;
  P.reserve(n);

  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);
  uniform_int_distribution<int> distribution(0, static_cast<int>(pow(n, 3) - 1));

  for (int i = 0; i < n; ++i) {
    int randomValue = distribution(generator);

    if (find(P.begin(), P.end(), randomValue) != P.end()) {
      --i;
      continue;
    }

    P.push_back(randomValue);
  }

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (P[i] > P[j]) {
        iter_swap(P.begin() + i, P.begin() + j);
        iter_swap(A.begin() + i, A.begin() + j);
      }
    }
  }
}

template <class value_type>
void PermuteBySorting(vector<value_type>& A)
{
  int n = A.size();

  vector<int> P;
  P.reserve(n);

  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);
  uniform_int_distribution<int> distribution(0, static_cast<int>(pow(n, 3) - 1));

  for (int i = 0; i < n; ++i) {
    P.push_back(distribution(generator));
  }

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (P[i] > P[j]) {
        iter_swap(P.begin() + i, P.begin() + j);
        iter_swap(A.begin() + i, A.begin() + j);
      }
    }
  }
}

template <class value_type>
string VectorToString(const vector<value_type>& targetVector)
{
  stringstream output;

  for (auto i : targetVector) {
    output << i << " ";
  }

  return output.str();
}

template <class vector_type>
vector<vector_type> GetAllPossiblePermutations(const vector_type& targetVector)
{
  vector<vector_type> permutations;

  if (targetVector.size() == 1) {
    // Root of recursion

    permutations.push_back(targetVector);
  } else {
    // Step of divide and conquer algorithm

    for (int i = 0, iend = targetVector.size(); i != iend; ++i) {
      vector_type tempVector = targetVector;
      tempVector.erase(tempVector.begin() + i);

      // Solve smaller case

      vector<vector_type>&& tempPermutations = GetAllPossiblePermutations(tempVector);

      // Merge

      for (int j = 0, jend = tempPermutations.size(); j != jend; ++j) {
        vector_type permutation = tempPermutations[j];
        permutation.insert(permutation.begin(), targetVector[i]);

        permutations.push_back(permutation);
      }
    }
  }

  #ifdef _DEBUG
    function<int (int)> factorial = [&](int n) -> int {
      return n > 1 ? n * factorial(n - 1) : 1;
    };

    int permutationsCount = factorial(targetVector.size());

    assert(permutations.size() == permutationsCount);
  #endif

  return permutations;
}

template <class vector_type>
int FindPermutation(const vector_type& needle, const vector<vector_type>& haystack)
{
  bool needleFound;

  for (int permutationIndex = 0, permutationIndexEnd = haystack.size(); permutationIndex != permutationIndexEnd; ++permutationIndex) {
    auto&& permutation = haystack[permutationIndex];

    assert(permutation.size() == needle.size());

    // Compare permutation with needle

    needleFound = true;

    for (int i = 0, iend = permutation.size(); i != iend; ++i) {
      if (permutation[i] != needle[i]) {
        needleFound = false;
        break;
      }
    }

    if (needleFound) {
      return permutationIndex;
    }
  }

  return -1;
}

template <class permutation_function_type>
void DoCalculations(const int& trialsCount, const vector<int>& initialVector, permutation_function_type permutationFunction)
{
  // Intermediate data

  const vector<vector<int>> possiblePermutations = move(GetAllPossiblePermutations(initialVector));
  vector<int> permutationOccurences(possiblePermutations.size(), 0);

  // Run experiment

  for (int i = 0; i < trialsCount; ++i) {
    auto tempVector = initialVector;
    permutationFunction(tempVector);
    int permutedVectorIndex = FindPermutation(tempVector, possiblePermutations);

    assert(permutedVectorIndex != -1);

    permutationOccurences[permutedVectorIndex]++;
  }

  // Display results

  unsigned expectedAvarageCount = trialsCount / possiblePermutations.size();

  cout << "All possible permutations count: " << possiblePermutations.size() << endl;
  cout << "Trials count: " << trialsCount << endl;
  cout << "Expected avarage count: " << expectedAvarageCount << endl << endl;

  cout << "Permutation occurance" << endl;

  for (size_t i = 0; i < possiblePermutations.size(); ++i) {
    cout << VectorToString(possiblePermutations[i]) << ": " << permutationOccurences[i]
      << ", deviation = " << abs(static_cast<int>(permutationOccurences[i] - expectedAvarageCount))
      << " / " << static_cast<double>(abs(static_cast<int>(permutationOccurences[i] - expectedAvarageCount))) / expectedAvarageCount
      << endl;
  }
  
  cout << endl;
}

int main()
{
  // Input data

  const int trialsCount = 10000;
  const vector<int> initialVector{1, 2, 3, 4};

  DoCalculations(trialsCount, initialVector, [](vector<int>& A) { PermuteBySorting(A); });
  DoCalculations(trialsCount, initialVector, [](vector<int>& A) { PermuteBySortingUnique(A); });

  return 0;
}
