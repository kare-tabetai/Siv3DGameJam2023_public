#pragma once

struct ElapsedTimer {
	double elapsed_sum = 0.0;

	void Reset() { elapsed_sum = 0.0; }
};
