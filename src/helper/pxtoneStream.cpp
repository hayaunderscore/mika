/*******************************************************************************************
*	pxtone is pretty cool
*	This file is under the public domain.
*******************************************************************************************/

#include "pxtoneStream.h"
#include "pxtnService.h"
#include "raylib.h"
#include <functional>

constexpr float i16_f32mul = 1.0f / 32768.0f;
static pxtnService* pxtn;

void pxtoneBaseCallback(void *buffer, unsigned int frames)
{
	const size_t bytes_to_do = frames * sizeof(int16_t) * 2;

	memset(buffer, 0, bytes_to_do);
	pxtn->Moo(buffer, bytes_to_do, NULL);
}

struct pxtnRaylibStream 
{
	pxtnService* pxtn;
	AudioStream stream;

	void updateCallback(void *buffer, unsigned int frames)
	{
		const size_t bytes_to_do = frames * sizeof(int16_t) * 2;

		memset(buffer, 0, bytes_to_do);
		this->pxtn->Moo(buffer, bytes_to_do, NULL);
	}
};

pxtnService* LoadPxtoneStream(const char *file, AudioStream &stream)
{
	// Init audio stream
	stream = LoadAudioStream(44100, 16, 2);

	// grab data from ptcop file
	int data_size;
	unsigned char* data = LoadFileData(file, &data_size);

	// Create a new service file
	if (pxtn != nullptr)
	{
		if (pxtn->evels != nullptr)
			pxtn->evels->Release();
		delete pxtn;
	}
	pxtn = new pxtnService();

	// init
	if (pxtn->init() != pxtnOK)
	{
		TraceLog(LOG_ERROR, "PXTONE: Could not create pxtn service!");
		delete pxtn;
		return nullptr;
	}

	// set quality
	if (!pxtn->set_destination_quality(2, 44100))
	{
		TraceLog(LOG_ERROR, "PXTONE: Could not set destination quality!");
		delete pxtn;
		return nullptr;
	}

	// load ptcop file
	pxtnDescriptor desc;

	if (!(desc.set_memory_r((void*)data, data_size) && pxtn->read(&desc) == pxtnOK && pxtn->tones_ready() == pxtnOK))
	{
		TraceLog(LOG_ERROR, "PXTONE: Could not load ptcop file!");
		pxtn->evels->Release();

		delete pxtn;
		return nullptr;
	}

	pxtnVOMITPREPARATION prep = pxtnVOMITPREPARATION();
	prep.flags |= pxtnVOMITPREPFLAG_loop;
	prep.start_pos_float = 0.0f;
	prep.master_volume = 0.8f;

	// prepare `Moo()`
	if (!pxtn->moo_preparation(&prep))
	{
		TraceLog(LOG_ERROR, "PXTONE: Could not prepare moo....");
		pxtn->evels->Release();

		delete pxtn;
		return nullptr;
	}

	// Set audio stream callback
	SetAudioStreamCallback(stream, pxtoneBaseCallback);
	TraceLog(LOG_INFO, "PXTONE: [%s] ptcop loaded successfully", file);

	return pxtn;
}

void ExplodePxtoneStream(AudioStream& stream, pxtnService* pxtn)
{
	if (pxtn != nullptr)
	{
		if (pxtn->evels != nullptr)
			pxtn->evels->Release();
		delete pxtn;
	}
	UnloadAudioStream(stream);
}