#include "LambdaSnailScreen.h"

void ULambdaSnailScreen::RequestCloseSelf() const
{
	if(OnRequestCloseSelf.IsBound())
	{
		OnRequestCloseSelf.Execute();	
	}
}