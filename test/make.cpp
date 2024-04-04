#include <stdio.h>
#include <gtest/gtest.h>
#include "Repositories/HorseRepository.h"

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}