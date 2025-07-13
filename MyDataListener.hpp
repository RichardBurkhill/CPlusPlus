#ifndef MY_DATA_LISTENER_H
#define MY_DATA_LISTENER_H

#include "MyData.h" // Your generated type header
#include "MyDataTypeSupport.h" // Your generated type support header
#include <iostream>

class MyDataListener : public DDS_DataReaderListener {
public:
    virtual void on_data_available(DDS_DataReader* reader) override;
    // Implement other listener callbacks if needed (on_requested_deadline_missed, etc.)
    virtual void on_requested_deadline_missed(DDS_DataReader* reader, const DDS_RequestedDeadlineMissedStatus& status) override {}
    virtual void on_requested_incompatible_qos(DDS_DataReader* reader, const DDS_RequestedIncompatibleQosStatus& status) override {}
    virtual void on_sample_rejected(DDS_DataReader* reader, const DDS_SampleRejectedStatus& status) override {}
    virtual void on_liveliness_changed(DDS_DataReader* reader, const DDS_LivelinessChangedStatus& status) override {}
    virtual void on_subscription_matched(DDS_DataReader* reader, const DDS_SubscriptionMatchedStatus& status) override {}
    virtual void on_sample_lost(DDS_DataReader* reader, const DDS_SampleLostStatus& status) override {}
};

#endif // MY_DATA_LISTENER_H