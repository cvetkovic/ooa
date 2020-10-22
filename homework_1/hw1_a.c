#include <stdio.h>

int main(int argc, char** argv)
{
	// a + b + c + d = a * b * c * d = 7.11
	const double totalCost = 7.11;
	const double increment = 0.01;
	size_t solutionCount = 0;
	size_t numberOfCalls = 0;

	for (double a = 0; a < totalCost; a += increment)
	{
		for (double b = 0; b < totalCost; b += increment)
		{
			for (double c = 0; c < totalCost; c += increment)
			{
				for (double d = 0; d < totalCost; d += increment)
				{
					double sum = a + b + c + d;
					double product = a * b * c * d;

					if ((sum == product) && sum == totalCost)
					{
						solutionCount++;
						printf("Solution found (a, b, c, d) = (%lf, %lf, %lf, %lf)\n", a, b, c, d);
					}

					numberOfCalls++;
				}
			}
		}
	}

	printf("Number of optimization function calls: %ld", numberOfCalls);

	return 0;
}
