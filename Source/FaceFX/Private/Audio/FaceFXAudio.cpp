/*******************************************************************************
The MIT License (MIT)
Copyright (c) 2015-2020 OC3 Entertainment, Inc. All rights reserved.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include "FaceFXAudio.h"
#include "FaceFX.h"
#include "FaceFXAnim.h"

#include "FaceFXAudioImplDefault.h"
#if WITH_CRIWARE_ATOM
#include "FaceFXAudioImplAtom.h"
#endif //WITH_CRIWARE_ATOM
#if WITH_WWISE
#include "FaceFXAudioImplWwise.h"
#endif //WITH_WWISE

#include "HAL/IConsoleManager.h"

TMap<EFaceFXAudioType, TSharedPtr<IFaceFXAudioCreater>> FFaceFXAudio::CreaterMap;

AActor* IFaceFXAudio::GetOwningActor() const
{
	UFaceFXCharacter* Character = Owner.Get();
	return Character ? Character->GetOwningActor() : nullptr;
}

TSharedPtr<IFaceFXAudio> FFaceFXAudio::Create(UFaceFXCharacter* Owner)
{
	if (FFaceFXAudio::CreaterMap.Num() <= 0)
	{
		// First time initialization
		FFaceFXAudio::CreaterMap.Add(EFaceFXAudioType::Default, MakeShareable(new FFaceFXAudioCreaterDefault));
#if WITH_WWISE
		FFaceFXAudio::CreaterMap.Add(EFaceFXAudioType::Wwise, MakeShareable(new FFaceFXAudioCreaterDefault));
#endif //WITH_WWISE
#if WITH_CRIWARE_ATOM
		FFaceFXAudio::CreaterMap.Add(EFaceFXAudioType::CriWareAtom, MakeShareable(new FFaceFXAudioCreaterAtom));
#endif //WITH_CRIWARE_ATOM
	}

	EFaceFXAudioType AudioType = FaceFX::GetAudioType();
	if (FFaceFXAudio::CreaterMap.Contains(AudioType))
	{
		return FFaceFXAudio::CreaterMap[AudioType]->Create(Owner);
	}

	return MakeShareable(new FFaceFXAudioDefault(Owner));
}

bool FFaceFXAudio::IsUsingSoundWaveAssets()
{
	return FaceFX::GetAudioType() == EFaceFXAudioType::Default;
}