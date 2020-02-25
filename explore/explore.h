#pragma once

#define S_EXPLORATION_OK                             0
#define E_EXPLORATION_BAD_RANGE                      1
#define E_EXPLORATION_PDF_RANKING_SIZE_MISMATCH      2

#include "explore_internal.h"

namespace exploration {
  /**
   * @brief Generates epsilon-greedy style exploration distribution.
   *
   * @tparam It Iterator type of the pre-allocated pmf. Must be a RandomAccessIterator.
   * @param epsilon Minimum probability used to explore among options. Each action is explored with at least epsilon/num_actions.
   * @param top_action Index of the exploit actions. This action will be get probability mass of 1-epsilon + (epsilon/num_actions).
   * @param pmf_first Iterator pointing to the pre-allocated beginning of the pmf to be generated by this function.
   * @param pmf_last Iterator pointing to the pre-allocated end of the pmf to be generated by this function.
   * @return int returns 0 on success, otherwise an error code as defined by E_EXPLORATION_*.
   */
  template<typename It>
  int generate_epsilon_greedy(float epsilon, uint32_t top_action, It pmf_first, It pmf_last);

  /**
   * @brief Generates softmax style exploration distribution.
   *
   * @tparam InputIt Iterator type of the input scores. Must be an InputIterator.
   * @tparam OutputIt Iterator type of the pre-allocated pmf. Must be a RandomAccessIterator.
   * @param lambda Lambda parameter of softmax.
   * @param scores_first Iterator pointing to beginning of the scores.
   * @param scores_last Iterator pointing to end of the scores.
   * @param pmf_first Iterator pointing to the pre-allocated beginning of the pmf to be generated by this function.
   * @param pmf_last Iterator pointing to the pre-allocated end of the pmf to be generated by this function.
   * @return int returns 0 on success, otherwise an error code as defined by E_EXPLORATION_*.
   */
  template<typename InputIt, typename OutputIt>
  int generate_softmax(float lambda, InputIt scores_first, InputIt scores_last, OutputIt pmf_first, OutputIt pmf_last);

  /**
   * @brief Generates an exploration distribution according to votes on actions.
   *
   * @tparam InputIt Iterator type of the input actions. Must be an InputIterator.
   * @tparam OutputIt Iterator type of the pre-allocated pmf. Must be a RandomAccessIterator.
   * @param top_actions_first Iterator pointing to the beginning of the top actions.
   * @param top_actions_last Iterator pointing to the end of the top actions.
   * @param pmf_first Iterator pointing to the pre-allocated beginning of the pmf to be generated by this function.
   * @param pmf_last Iterator pointing to the pre-allocated end of the pmf to be generated by this function.
   * @return int returns 0 on success, otherwise an error code as defined by E_EXPLORATION_*.
   */
  template<typename InputIt, typename OutputIt>
  int generate_bag(InputIt top_actions_first, InputIt top_actions_last, OutputIt pmf_first, OutputIt pmf_last);

  /**
   * @brief Updates the pmf to ensure each action is explored with at least minimum_uniform/num_actions.
   *
   * @tparam It Iterator type of the pmf. Must be a RandomAccessIterator.
   * @param minimum_uniform The minimum amount of uniform distribution to impose on the pmf.
   * @param update_zero_elements If true elements with zero probability are updated, otherwise those actions will be unchanged.
   * @param pmf_first Iterator pointing to the pre-allocated beginning of the pmf to be generated by this function.
   * @param pmf_last Iterator pointing to the pre-allocated end of the pmf to be generated by this function.
   * @return int returns 0 on success, otherwise an error code as defined by E_EXPLORATION_*.
   */
  template<typename It>
  int enforce_minimum_probability(float minimum_uniform, bool update_zero_elements, It pmf_first, It pmf_last);

  /**
   * @brief Sample an index from the provided pmf. If the pmf is not normalized it will be updated in-place.
   *
   * @tparam InputIt Iterator type of the pmf. Must be a RandomAccessIterator.
   * @param seed The seed for the pseudo-random generator.
   * @param pmf_first Iterator pointing to the beginning of the pmf.
   * @param pmf_last Iterator pointing to the end of the pmf.
   * @param chosen_index returns the chosen index.
   * @return int returns 0 on success, otherwise an error code as defined by E_EXPLORATION_*.
   */
  template<typename It>
  int sample_after_normalizing(uint64_t seed, It pmf_first, It pmf_last, uint32_t& chosen_index);

  /**
   * @brief Sample an index from the provided pmf.  If the pmf is not normalized it will be updated in-place.
   *
   * @tparam It Iterator type of the pmf. Must be a RandomAccessIterator.
   * @param seed The seed for the pseudo-random generator. Will be hashed using MURMUR hash.
   * @param pmf_first Iterator pointing to the beginning of the pmf.
   * @param pmf_last Iterator pointing to the end of the pmf.
   * @param chosen_index returns the chosen index.
   * @return int returns 0 on success, otherwise an error code as defined by E_EXPLORATION_*.
   */
  template<typename It>
  int sample_after_normalizing(const char* seed, It pmf_first, It pmf_last, uint32_t& chosen_index);

  /**
   * @brief Sample an index from the provided pmf. If the pmf is not normalized it will be updated in-place.
   *
   * @tparam InputIt Iterator type of the pmf. Must be a RandomAccessIterator.
   * @param seed The seed for the pseudo-random generator.
   * @param pmf_first Iterator pointing to the beginning of the pmf.
   * @param pmf_last Iterator pointing to the end of the pmf.
   * @param chosen_index returns the chosen index.
   * @return int returns 0 on success, otherwise an error code as defined by E_EXPLORATION_*.
   */
  template <typename It>
  int sample_without_normalizing(uint64_t seed, It pmf_first, It pmf_last, uint32_t& chosen_index);

  /**
   * @brief Sample an index from the provided pmf.  If the pmf is not normalized it will be updated in-place.
   *
   * @tparam It Iterator type of the pmf. Must be a RandomAccessIterator.
   * @param seed The seed for the pseudo-random generator. Will be hashed using MURMUR hash.
   * @param pmf_first Iterator pointing to the beginning of the pmf.
   * @param pmf_last Iterator pointing to the end of the pmf.
   * @param chosen_index returns the chosen index.
   * @return int returns 0 on success, otherwise an error code as defined by E_EXPLORATION_*.
   */
  template <typename It>
  int sample_without_normalizing(const char* seed, It pmf_first, It pmf_last, uint32_t& chosen_index);
  
  /**
   * @brief Sample an index from the provided pdf. If the pdf is not normalized it will be updated in-place.
   *
   * @tparam InputIt Iterator type of the pdf. Must be a RandomAccessIterator.
   * @param seed The seed for the pseudo-random generator.
   * @param pdf_first Iterator pointing to the beginning of the pdf.
   * @param pdf_last Iterator pointing to the end of the pdf.
   * @param chosen_index returns the chosen index.
   * @return int returns 0 on success, otherwise an error code as defined by E_EXPLORATION_*.
   */
  template<typename It>
  int sample_pdf(
      uint64_t seed, It pdf_first, It pdf_last, float range_min, float range_max, float& chosen_value);

  /**
   * @brief Sample an index from the provided pdf.  If the pdf is not normalized it will be updated in-place.
   *
   * @tparam It Iterator type of the pdf. Must be a RandomAccessIterator.
   * @param seed The seed for the pseudo-random generator. Will be hashed using MURMUR hash.
   * @param pdf_first Iterator pointing to the beginning of the pdf.
   * @param pdf_last Iterator pointing to the end of the pdf.
   * @param chosen_index returns the chosen index.
   * @return int returns 0 on success, otherwise an error code as defined by E_EXPLORATION_*.
   */
  template<typename It>
  int sample_pdf(
      const char* seed, It pdf_first, It pdf_last, float range_min, float range_max, float& chosen_value);

  /**
  * @brief Swap the first value with the chosen index.
  *
  * @tparam ActionIt Iterator type of the action. Must be a forward_iterator.
  * @param action_first Iterator pointing to the beginning of the pdf.
  * @param action_last Iterator pointing to the end of the pdf.
  * @param chosen_index The index value that should be swapped with the first element
  * @return int returns 0 on success, otherwise an error code as defined by E_EXPLORATION_*.
  */
  template<typename ActionIt>
  int swap_chosen(ActionIt action_first, ActionIt action_last, uint32_t chosen_index);
}
