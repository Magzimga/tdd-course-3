/*
We have to develop a coffee machine with TDD. The machine uses the automated source of ingredients, which implements the interface ISourceOfIngredients.

Our machine should be able to produce the coffee accordingly to the next receipts:
- americano: water & coffee 1/3 & 2/3 or 1/4 & 3/4. Water temp 60C
- cappuccino - milk & coffee & milk foam 1/3 & 1/3 & 1/3. Water temp 80C
- latte - milk & coffee & milk foam 1/4 & 1/2 & 1/4. Water temp 90C
- marochino - chocolate & coffee & milk foam, 1:4, 1:4, 1:4 and 1:4 is empty

We have 2 possible sizes of the cup:
- little 100 gram
- big 140 gram

Implement worked coffee machine using ISourceOfIngredients to controll the process of coffee production.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class ISourceOfIngredients
{
public:
    virtual ~ISourceOfIngredients() {}
    virtual void SetCupSize(int gram) = 0;
    virtual void AddWater(int gram, int temperature) = 0;
    virtual void AddSugar(int gram) = 0;
    virtual void AddCoffee(int gram) = 0;
    virtual void AddMilk(int gram) = 0;
    virtual void AddMilkFoam(int gram) = 0;
    virtual void AddChocolate(int gram) = 0;
    virtual void AddCream(int gram) = 0;
};

enum CoffeType
{
    Americano,
    Cappuchino,
    Latte,
    Marochino
};

enum CupSize
{
    Little = 100,
    Big = 140
};

struct CoffeeReceipt
{
int coffe = 0;
int sugar = 0;
int milk = 0;
int milkFoam = 0;
int chocolate = 0;
int cream = 0;
int waterTemp = 0;
bool AddWater = true;
};

class SourceOfIngredientsMock : public ISourceOfIngredients
{
    // ISourceOfIngredients interface
public:
    MOCK_METHOD1(SetCupSize, void (int gram));
    MOCK_METHOD2(AddWater, void (int gram, int temperature));
    MOCK_METHOD1(AddSugar, void (int gram));
    MOCK_METHOD1(AddCoffee, void (int gram));
    MOCK_METHOD1(AddMilk, void (int gram));
    MOCK_METHOD1(AddMilkFoam, void (int gram));
    MOCK_METHOD1(AddChocolate, void (int gram));
    MOCK_METHOD1(AddCream, void (int gram));
};

class CoffeMachine
{
public:
    void MakeSomeCoffe(ISourceOfIngredients& ingredientsSource, CoffeType type, CupSize size)
    {
        ingredientsSource.SetCupSize(static_cast<int>(size));
        ingredientsSource.AddCoffee(static_cast<int>(size)/3);
    }
};

TEST(CoffeMachine, SetsRightCupSizeForBig)
{
    SourceOfIngredientsMock source;
    CoffeMachine coffeMachine;
    EXPECT_CALL(source, SetCupSize(140)).Times(1);
    coffeMachine.MakeSomeCoffe(source, Americano, Big);
}
TEST(CoffeMachine, SetsRightCupSizeForLittle)
{
    SourceOfIngredientsMock source;
    CoffeMachine coffeMachine;
    EXPECT_CALL(source, SetCupSize(100)).Times(1);
    coffeMachine.MakeSomeCoffe(source, Americano, Little);
}

TEST(CoffeMachine, AmericanoLittleAdd33GgramsOfCoffe)
{
    SourceOfIngredientsMock source;
    CoffeMachine coffeMachine;
    EXPECT_CALL(source, SetCupSize(100)).Times(1);
    EXPECT_CALL(source, AddCoffee(33)).Times(1);
    coffeMachine.MakeSomeCoffe(source, Americano, Little);
}
TEST(CoffeMachine, AmericanoLittleAddWaterToFullCup)
{
    SourceOfIngredientsMock source;
    CoffeMachine coffeMachine;
    EXPECT_CALL(source, SetCupSize(100)).Times(1);
    EXPECT_CALL(source, AddWater(67, 60)).Times(1);
    coffeMachine.MakeSomeCoffe(source, Americano, Little);
}
