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
#include <map>
#include <utility>
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

struct CoffeeOrder
{
    CupSize size = Little;
    CoffeType type = Americano;
    CoffeeOrder(CoffeType type_in, CupSize size_in)
        :size(size_in), type(type_in)
    {}
};

struct CoffeeReceipt
{
int coffePart = 0;
int sugarPart = 0;
int milkPart = 0;
int milkFoamPart = 0;
int chocolatePart = 0;
int creamPart = 0;
int waterTemp = 0;
bool addWater = true;
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
        int remainingSize = static_cast<int>(size);
        ingredientsSource.SetCupSize(remainingSize);

        std::map<CoffeeOrder, CoffeeReceipt> receiptBook;
        CoffeeReceipt americano;
        americano.coffePart = 33;
        americano.waterTemp = 60;
        CoffeeOrder order
        order.type = Americano;
        order.size = Little;
        receiptBook.insert(std::make_pair(CoffeeOrder(Americano, Little), americano));

        order.type = Americano;
        order.size = Big;
        CoffeeReceipt americanoBig;
        americanoBig.coffePart = 35;
        americanoBig.waterTemp = 60;
        receiptBook.insert({order, americanoBig});


        CoffeeReceipt receipt = receiptBook[{type, size}];

        ingredientsSource.AddCoffee(remainingSize/receipt.coffePart);
        remainingSize -= remainingSize/receipt.coffePart;
        ingredientsSource.AddWater(remainingSize, receipt.waterTemp);
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
TEST(CoffeMachine, AmericanoBigFullReceipt)
{
    SourceOfIngredientsMock source;
    CoffeMachine coffeMachine;
    EXPECT_CALL(source, SetCupSize(140)).Times(1);
    EXPECT_CALL(source, AddCoffee(35)).Times(1);
    EXPECT_CALL(source, AddWater(105, 60)).Times(1);
    coffeMachine.MakeSomeCoffe(source, Americano, Big);
}
