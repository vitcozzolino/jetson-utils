/*
 * Copyright (c) 2017, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "memoryManager.h"
#include "cudaMappedMemory.h"

memoryManager* memoryManager::Create( uint8_t threshold )
{
	memoryManager* mm = new memoryManager();

	mm->threshold = threshold;
	mm->usedPointerList = new std::list<usedPointer>;

	return mm;
}

int memoryManager::addUsedPointer( void* ptr ) {

	usedPointer t;
	t.ptr = ptr;

	usedPointerList->push_back(t);

	return 0;
}

bool memoryManager::checkThreshold(){
	return usedPointerList->size() >= threshold;
}

int memoryManager::deallocatePointers( )
{
	if(checkThreshold()){
		printf("Cleaning memory procedure started\n");
		for (std::list<usedPointer>::iterator it=usedPointerList->begin(); it != usedPointerList->end(); ++it) {
			if( CUDA_FAILED(cudaFreeHost(it->ptr)) )
			{
				printf("failed to free CUDA mapped memory with cudaFreeHost()\n");
				return -1;
			}
		}
		// printf("Cleaning memory");
		usedPointerList->clear();
	}

	return 0;
}
