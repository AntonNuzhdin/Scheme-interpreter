#pragma once

#include <memory>

#include "object.h"
#include <tokenizer.h>

std::shared_ptr<Object> Read(Tokenizer* tokenizer);

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer);

std::shared_ptr<Object> ReadRun(Tokenizer* tokenizer);