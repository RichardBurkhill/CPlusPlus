#include "MyDataListener.hpp"

void MyDataListener::on_data_available(DDS_DataReader* reader) {
    MyDataTypeSupport::MyDataDataReader* myDataReader = MyDataTypeSupport::MyDataDataReader::narrow(reader);
    if (myDataReader == nullptr) {
        std::cerr << "DataReader narrow error." << std::endl;
        return;
    }

    MyData data;
    DDS_SampleInfo info;
    DDS_ReturnCode_t retcode = myDataReader->take_next_sample(data, info);

    if (retcode == DDS_RETCODE_NO_DATA) {
        return;
    } else if (retcode != DDS_RETCODE_OK) {
        std::cerr << "take_next_sample error: " << retcode << std::endl;
        return;
    }

    if (info.valid_data) {
        std::cout << "Received MyData: ID=" << data.id << ", Message='" << data.message << "'" << std::endl;
    }
}
