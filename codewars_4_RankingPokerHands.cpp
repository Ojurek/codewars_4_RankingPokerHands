#include <cassert>
#include <array>
#include <algorithm>
#include <iostream>

enum class HandCategories
{
  StraightFlush = 0,
  FourOfKind,
  FullHouse,
  Flush,
  Straight,
  ThreeOfKind,
  TwoPairs,
  Pair,
  Highcard
};

struct PokerHand
{
private:
  std::array<int, 5> card;
  std::array<int, 15> kind_count = {};
  std::array<int, 15> kind_count_sort;
  int most_numerous_kind;
  int second_numerous_kind;
  bool flush = true;
  bool straight = true;
  HandCategories category;
  HandCategories setCategory();

public:
  std::array<int, 5> card_corrected;
  PokerHand(const char *pokerhand)
  {

    for (int i = 0; i < 5; i++)
    {
      switch (pokerhand[i * 3])
      {
      case 'T':
        card[i] = 10;
        break;
      case 'J':
        card[i] = 11;
        break;
      case 'Q':
        card[i] = 12;
        break;
      case 'K':
        card[i] = 13;
        break;
      case 'A':
        card[i] = 14;
        break;
      default:
        card[i] = pokerhand[i * 3] - 48;
        break;
      }
      kind_count[card[i]]++;
      if (pokerhand[1] != pokerhand[i * 3 + 1])
      {
        flush = false;
      }
    }
    std::sort(card.begin(), card.end(), std::greater<int>());
    for (int i = 0; i < 4; i++)
    {
      if ((card[i]) != (card[i + 1] + 1))
      {
        straight = false;
        break;
      }
    }

    //sort by kind
    for (int i = 0; i < 5; i++)
    {
      card_corrected[i] = card[i] + kind_count[card[i]] * 100;
    }

    std::sort(card_corrected.begin(), card_corrected.end(), std::greater<int>());

    for (int i = 0; i < 5; i++)
    {
      card_corrected[i] = card_corrected[i] % 100;
    }

    kind_count_sort = kind_count;
    std::sort(kind_count_sort.begin(), kind_count_sort.end(), std::greater<int>());
    most_numerous_kind = kind_count_sort[0];
    second_numerous_kind = kind_count_sort[1];

    //set category
    category = setCategory();
    std::cout << std::endl
              << "card" << std::endl;
    for (int i = 0; i < 5; i++)
    {
      std::cout << card[i] << ", ";
    }
    std::cout << std::endl
              << "card_corrected" << std::endl;
    for (int i = 0; i < 5; i++)
    {
      std::cout << card_corrected[i] << ", ";
    }
    std::cout << std::endl
              << "kind_count" << std::endl;
    for (int i = 0; i < 15; i++)
    {
      std::cout << kind_count[i] << ", ";
    }
    std::cout << std::endl
              << "kind_count_sort" << std::endl;
    for (int i = 0; i < 15; i++)
    {
      std::cout << kind_count_sort[i] << ", ";
    }
    std::cout << "flush " << flush << std::endl;
    std::cout << "straight " << straight << std::endl;
  }
  HandCategories getCategory() const
  {
    return category;
  }
};

HandCategories PokerHand::setCategory()
{

  if (most_numerous_kind == 2)
  {
    if (second_numerous_kind == 2)
    {
      return HandCategories::TwoPairs;
    }
    else
    {
      return HandCategories::Pair;
    }
  }

  if (most_numerous_kind == 3)
  {
    if (second_numerous_kind == 2)
    {
      return HandCategories::FullHouse;
    }
    else
    {
      return HandCategories::ThreeOfKind;
    }
  }

  if (most_numerous_kind == 4)
  {
    return HandCategories::FourOfKind;
  }

  if ((straight == true) && (flush == true))
  {
    return HandCategories::StraightFlush;
  }

  if (straight == true)
  {
    return HandCategories::Straight;
  }

  if (flush == true)
  {
    return HandCategories::Flush;
  }

  return HandCategories::Highcard;
}

enum class Result
{
  Win,
  Loss,
  Tie
};

Result compare(const PokerHand &player, const PokerHand &opponent)
{
  if ((static_cast<int>(player.getCategory())) < (static_cast<int>(opponent.getCategory())))
  {
    return Result::Win;
  }

  if ((static_cast<int>(player.getCategory())) > (static_cast<int>(opponent.getCategory())))
  {
    return Result::Loss;
  }

  for (int i = 0; i < 5; i++)
  {
    if (player.card_corrected[i] > opponent.card_corrected[i])
    {
      return Result::Win;
    }

    if (player.card_corrected[i] < opponent.card_corrected[i])
    {
      return Result::Loss;
    }
  }

  return Result::Tie;
}

bool run_test_hands(const PokerHand &player, const PokerHand &opponent, Result outcome)
{
  return outcome == compare(player, opponent);
}

bool run_test(const char *player, const char *opponent, Result outcome)
{
  return run_test_hands(PokerHand(player), PokerHand(opponent), outcome);
}

bool check_category(const char *player, HandCategories outcome)
{
  return outcome == PokerHand(player).getCategory();
}

int main()
{
  assert(check_category("2H 3H 4H 5H 6H", HandCategories::StraightFlush));
  assert(check_category("JS JD AD JC JH", HandCategories::FourOfKind));
  assert(check_category("2S AH 2H AS AC", HandCategories::FullHouse));
  assert(check_category("2H 3H 5H 6H 7H", HandCategories::Flush));
  assert(check_category("2S 3H 4H 5S 6C", HandCategories::Straight));
  assert(check_category("AH AC 5H 6H AS", HandCategories::ThreeOfKind));
  assert(check_category("2H 6H 4H 2H 6H", HandCategories::TwoPairs));
  assert(check_category("2H 5H 4D 2H 6H", HandCategories::Pair));
  assert(check_category("2H 8D 4H 5H 6H", HandCategories::Highcard));

  assert(run_test("2H 3H 4H 5H 6H", "AS AD AC AH JD", Result::Win));  // "Straight flush wins of 4 of a kind"
  assert(run_test("2S AH 2H AS AC", "JS JD JC JH AD", Result::Loss)); // "4 Of a kind wins of full house"
  assert(run_test("2S AH 2H AS AC", "2H 3H 5H 6H 7H", Result::Win));  // "Full house wins of flush"
  assert(run_test("2H 3H 5H 6H 7H", "2S 3H 4H 5S 6C", Result::Win));  // "Flush wins of straight"
  assert(run_test("2S 3H 4H 5S 6C", "3D 4C 5H 6H 2S", Result::Tie));  // "Equal straight is tie"
  assert(run_test("2S 3H 4H 5S 6C", "AH AC 5H 6H AS", Result::Win));  // "Straight wins of three of a kind"
  assert(run_test("2S 3H 4H 5S 6C", "AH AC 5H 6H AS", Result::Win));  // "Low-ace straight wins of three of a kind"
  assert(run_test("2S 2H 4H 5S 4C", "AH AC 5H 6H AS", Result::Loss)); // "3 Of a kind wins of two pair"
  assert(run_test("2S 2H 4H 5S 4C", "AH AC 5H 6H 7S", Result::Win));  // "2 Pair wins of pair"
  assert(run_test("2S AH 4H 5S 6C", "AD 4C 5H 6H 2C", Result::Tie));  // "Equal cards is tie"
  assert(run_test("2S AH 4H 5S KC", "AH AC 5H 6H 7S", Result::Loss)); // "Pair wins of nothing"
  assert(run_test("6S AD 7H 4S AS", "AH AC 5H 6H 7S", Result::Loss)); // "Highest pair wins"
  assert(run_test("2S 3H 6H 7S 9C", "7H 3C TH 6H 9S", Result::Loss)); // "Highest card loses"
  assert(run_test("4S 5H 6H TS AC", "3S 5H 6H TS AC", Result::Win));  // "Highest card wins"
  assert(run_test("2H 3H 4H 5H 6H", "KS AS TS QS JS", Result::Loss)); // "Highest straight flush wins"
  assert(run_test("AS AH 2H AD AC", "JS JD JC JH 3D", Result::Win));  // "Highest 4 of a kind wins"
  assert(run_test("AS 3S 4S 8S 2S", "2H 3H 5H 6H 7H", Result::Win));  // "Highest flush wins"
}