/****************************************************************************
 Copyright (c) 2020-2021 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
****************************************************************************/

#include "base/CoreStd.h"
#include "base/threading/MessageQueue.h"

#include "DeviceAgent.h"
#include "TextureAgent.h"

namespace cc {
namespace gfx {

TextureAgent::TextureAgent(Texture *actor)
: Agent<Texture>(actor) {
    _typedID = generateObjectID<decltype(this)>();
}

TextureAgent::~TextureAgent() {
    ENQUEUE_MESSAGE_1(
        DeviceAgent::getInstance()->getMessageQueue(),
        TextureDestruct,
        actor, _actor,
        {
            CC_SAFE_DELETE(actor);
        });
}

void TextureAgent::doInit(const TextureInfo &info) {
    ENQUEUE_MESSAGE_2(
        DeviceAgent::getInstance()->getMessageQueue(),
        TextureInit,
        actor, getActor(),
        info, info,
        {
            actor->initialize(info);
        });
}

void TextureAgent::doInit(const TextureViewInfo &info) {
    TextureViewInfo actorInfo = info;
    actorInfo.texture         = static_cast<TextureAgent *>(info.texture)->getActor();

    ENQUEUE_MESSAGE_2(
        DeviceAgent::getInstance()->getMessageQueue(),
        TextureViewInit,
        actor, getActor(),
        info, actorInfo,
        {
            actor->initialize(info);
        });
}

void TextureAgent::doDestroy() {
    ENQUEUE_MESSAGE_1(
        DeviceAgent::getInstance()->getMessageQueue(),
        TextureDestroy,
        actor, getActor(),
        {
            actor->destroy();
        });
}

void TextureAgent::doResize(uint width, uint height, uint /*size*/) {
    ENQUEUE_MESSAGE_3(
        DeviceAgent::getInstance()->getMessageQueue(),
        TextureResize,
        actor, getActor(),
        width, width,
        height, height,
        {
            actor->resize(width, height);
        });
}

} // namespace gfx
} // namespace cc
