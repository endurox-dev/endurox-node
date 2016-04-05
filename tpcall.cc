
#include <stdexcept>
#include <sstream>
#include <string>

#include <nan.h>

#include <atmi.h>
#include <fml.h>
#include <ndebug.h>

using std::domain_error;
using std::stringstream;

using namespace v8;

#define SUCCEED 0
#define FAIL -1

char * nodeValueToCString(Handle<Value> value) {

	NanScope();

	NanUtf8String *str = new NanUtf8String(value);
	return **str;
}

FBFR * parseRequest(Local<Object> obj) {

	FBFR *p_fb = NULL;
	const Local<Array> props = obj->GetPropertyNames();
	int len = props->Length();
	int i;
	FLDOCC occ;
	FLDID fldid;

	if (NULL == (p_fb = (FBFR *)tpalloc("UBF", NULL, 9216))) {

		stringstream sstm;

		sstm << "comment {tpinit failed} ";
		sstm << "type {" << "T" << "} ";
		sstm << "code {" << tperrno << "} ";
		sstm << "message {" << tpstrerror(tperrno) << "}";

		throw domain_error(sstm.str());
	}

	for (i = 0; i < len; i++) {

		Handle<Value> field = props->Get(NanNew<Number>(i));
		Local<Value> fieldValuesObj = obj->Get(field);
		Local<Array> fieldValues = Local<Array>::Cast(fieldValuesObj->ToObject());

		int occurances = fieldValues->Length();

		for (occ = 0; occ < occurances; occ++) {

			Handle<Value> value = fieldValues->Get(NanNew<Number>(occ));

			char *fldname = nodeValueToCString(field);

			printf("looking up... [%s]	\n", fldname);

			fldid = Bfldid(fldname);

			if (BADFLDID == fldid) {

				stringstream sstm;

				sstm << "comment {failed to lookup field " << fldname << "} ";
				sstm << "type {" << "B" << "} ";
				sstm << "code {" << Berror << "} ";
				sstm << "message {" << Bstrerror(Berror) << "}";

				tpfree((char*)p_fb);
				delete fldname;

				throw domain_error(sstm.str());
			}

			delete fldname;

			if (value->IsString()) {

				char *buf = nodeValueToCString(value);

				if (SUCCEED != CBchg(p_fb, fldid, occ, buf, 0L, FLD_STRING)) {

					stringstream sstm;

					sstm << "comment {failed to setup FB} ";
					sstm << "type {" << "B" << "} ";
					sstm << "code {" << Berror << "} ";
					sstm << "message {" << Bstrerror(Berror) << "}";

					tpfree((char*)p_fb);
					delete buf;

					throw domain_error(sstm.str());
				}

				delete buf;
			}
			else if (value->IsNumber()) {

				double itemNumberValue = value->NumberValue();

				if (SUCCEED != CBchg(p_fb, fldid, occ, (char *)&itemNumberValue, 0L, FLD_DOUBLE)) {

					stringstream sstm;

					sstm << "comment {failed to setup FB} ";
					sstm << "type {" << "B" << "} ";
					sstm << "code {" << Berror << "} ";
					sstm << "message {" << Bstrerror(Berror) << "}";

					tpfree((char*)p_fb);

					throw domain_error(sstm.str());
				}
			}
		}
	}

	return p_fb;
}

#define TMP_BUF_LEN 4096

Local<Object> buildResponse(FBFR *p_fb) {

	FLDID fldid = FIRSTFLDID;
	FLDLEN flen = 0;
	FLDOCC	occ;
	char buf[TMP_BUF_LEN];
	short dtype;
	double d;
	FLDID prevfldid = BADFLDID;
	int occs;
	char *fldname;

	Local<Object> obj = NanNew<Object>();

	while(Bnext(p_fb, &fldid, &occ, NULL, &flen)) {

		if (prevfldid != fldid) {

			Handle<Array> occCont = Array::New(0);

			// repeat after the cycle

			if (prevfldid != BADFLDID) {
				// add array with key fldname
			}

			dtype = Bfldtype(fldid);
			occs = Boccur(p_fb, fldid);
			fldname =  Bfname(fldid);

			obj->Set(NanNew(fldname), occCont);

			prevfldid = fldid;
		}

		flen = sizeof(buf);

		/*
		Support for Binary Fields

		if (FLD_CARRAY == dtype) {

			if (FLD_CARRAY == Bldtype(fldid) && SUCCEED != CBget(p_fb, fldid, occ, strbuf, &flen, FLD_CARRAY)) {

				fprintf(stderr, "Failed to setup FB code: [%d] msg: [%s]\n", Berror, Bstrerror(Berror));
				ret = FAIL;
				goto out;
			}

			NDRX_DBG_DUMP(dbg_syserr,"Carray value: ", strbuf,flen);
		}
		*/

		if (FLD_STRING == dtype || FLD_CHAR == dtype || FLD_CARRAY == dtype) {

			if (SUCCEED != CBget(p_fb, fldid, occ, buf, 0L, FLD_STRING)) {

				stringstream sstm;

				sstm << "comment {failed to setup FB} ";
				sstm << "type {" << "B" << "} ";
				sstm << "code {" << Berror << "} ";
				sstm << "message {" << Bstrerror(Berror) << "}";

				tpfree((char*)p_fb);

				throw domain_error(sstm.str());
			}
			else {

				Handle<Array> arr = Local<Array>::Cast(obj->Get(NanNew(fldname)));
				arr->Set(arr->Length(), NanNew(buf));

				printf("got string key: [%s] occ [%d] value [%s]\n", fldname, occ, buf);
			}
		}
		else {

			if (SUCCEED != CBget(p_fb, fldid, occ, (char *)&d, 0L, FLD_DOUBLE)) {

				stringstream sstm;

				sstm << "comment {failed to setup FB} ";
				sstm << "type {" << "B" << "} ";
				sstm << "code {" << Berror << "} ";
				sstm << "message {" << Bstrerror(Berror) << "}";

				tpfree((char*)p_fb);

				throw domain_error(sstm.str());
			}
			else {

				Handle<Array> arr = Local<Array>::Cast(obj->Get(NanNew(fldname)));
				arr->Set(arr->Length(), NanNew(d));

				printf("got double key: [%s] occ [%d] value [%lf]\n", fldname, occ, d);
			}
		}
	}

	tpfree((char *)p_fb);

	return obj;
}

class TpCallWorker : public NanAsyncWorker {

public:

	TpCallWorker(char *srvcnm, FBFR *p_fb, NanCallback *callback) :
		srvcnm(srvcnm), p_fb(p_fb), NanAsyncWorker(callback) {}

	~TpCallWorker() {
		delete srvcnm;
	}

	void Execute() {

		int ret = SUCCEED;
		char result[512] = { 0 };
		long rsplen;

		printf("request buffer:\n");
		Bprint(p_fb);

		if (FAIL == tpcall(srvcnm, (char *)p_fb, 0L, (char **)&p_fb, &rsplen, 0)) {

			stringstream sstm;

			sstm << "comment {" << srvcnm << " failed" << "} ";
			sstm << "type {" << "T" << "} ";
			sstm << "code {" << tperrno << "} ";
			sstm << "message {" << tpstrerror(tperrno) << "}";

			error = sstm.str();
		}
		else {

			printf("response buffer:\n");
			Bprint(p_fb);
		}
	}

	void HandleOKCallback() {

		NanScope();


		// if there's an error and it's not a program error

		if (error.size() &&
			error.find("type {T}") == std::string::npos &&
			error.find("code {11}") == std::string::npos) {

			Local<Value> argv[] = { NanError(error.c_str()) };
			printf("\n---END OF REQUEST ---\n\n");
			callback->Call(2, argv);

			return;
		}


		// success and program errors have FML response data

		try {

			Local<Value> argv[] = { NanNull(), buildResponse(p_fb) };
			printf("\n---END OF REQUEST ---\n\n");
			callback->Call(2, argv);
		}
		catch (domain_error e) {

			Local<Value> argv[] = { NanError(e.what()) };
			printf("\n---END OF REQUEST ---\n\n");
			callback->Call(2, argv);
		}
	}

private:

	char *srvcnm;
	FBFR *p_fb;
	std::string error;
};

NAN_METHOD(TpCall) {

	NanScope();

	char *srvcnm = nodeValueToCString(args[0]);
	FBFR *p_fb = NULL;

	Local<Object> obj = args[1]->ToObject();
	NanCallback *callback = new NanCallback(args[2].As<Function>());

	try {
		p_fb = parseRequest(obj);
	}
	catch (domain_error e) {

		Local<Value> argv[] = { NanError(e.what()) };

		callback->Call(2, argv);
		NanReturnUndefined();
	}

	NanAsyncQueueWorker(new TpCallWorker(srvcnm, p_fb, callback));

	NanReturnUndefined();
}

void Init(Handle<Object> exports, Handle<Object> module) {
	exports->Set(NanNew<String>("tpcall"), NanNew<FunctionTemplate>(TpCall)->GetFunction());
}

NODE_MODULE(tpcall, Init)
