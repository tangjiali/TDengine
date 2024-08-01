package com.taosdata.example;

import com.taosdata.jdbc.tmq.ConsumerRecord;
import com.taosdata.jdbc.tmq.ConsumerRecords;
import com.taosdata.jdbc.tmq.ReferenceDeserializer;
import com.taosdata.jdbc.tmq.TaosConsumer;

import java.sql.SQLException;
import java.sql.Timestamp;
import java.time.Duration;
import java.util.Collections;
import java.util.List;
import java.util.Properties;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicBoolean;

// ANCHOR: consumer_demo
public abstract class AbsWsConsumerLoop {
    private final TaosConsumer<ResultBean> consumer;
    private final List<String> topics;
    private final AtomicBoolean shutdown;
    private final CountDownLatch shutdownLatch;

    public AbsWsConsumerLoop() throws SQLException {
// ANCHOR: create_consumer
Properties config = new Properties();
config.setProperty("td.connect.type", "ws");
config.setProperty("bootstrap.servers", "localhost:6041");
config.setProperty("auto.offset.reset", "latest");
config.setProperty("msg.with.table.name", "true");
config.setProperty("enable.auto.commit", "true");
config.setProperty("auto.commit.interval.ms", "1000");
config.setProperty("group.id", "group2");
config.setProperty("client.id", "1");
config.setProperty("value.deserializer", "com.taosdata.example.AbsConsumerLoopWs$ResultDeserializer");
config.setProperty("value.deserializer.encoding", "UTF-8");

try {
    this.consumer = new TaosConsumer<>(config);
} catch (SQLException ex) {
    // handle any errors, please refer to the JDBC specifications for detailed exceptions info
    System.out.println("Failed to create ws consumer with " + config.getProperty("bootstrap.servers") + " ErrCode:" + ex.getErrorCode() + "; ErrMessage: " + ex.getMessage());
    throw new SQLException("Failed to create consumer", ex);
}
// ANCHOR_END: create_consumer

        this.topics = Collections.singletonList("topic_meters");
        this.shutdown = new AtomicBoolean(false);
        this.shutdownLatch = new CountDownLatch(1);
    }

    public abstract void process(ResultBean result);

    public void pollData() throws SQLException {
        try {
            // Subscribe to the topic
            consumer.subscribe(topics);

            while (!shutdown.get()) {
                // poll data
                ConsumerRecords<ResultBean> records = consumer.poll(Duration.ofMillis(100));
                for (ConsumerRecord<ResultBean> record : records) {
                    ResultBean bean = record.value();
                    // process data here
                    process(bean);
                }
            }
            // unsubscribe the topics
            consumer.unsubscribe();
        } finally {
            // close the consumer
            consumer.close();
            shutdownLatch.countDown();
        }
    }

    public void shutdown() throws InterruptedException {
        shutdown.set(true);
        shutdownLatch.await();
    }

    public static class ResultDeserializer extends ReferenceDeserializer<ResultBean> {

    }

    // use this class to define the data structure of the result record
    public static class ResultBean {
        private Timestamp ts;
        private double current;
        private int voltage;
        private double phase;
        private int groupid;
        private String location;

        public Timestamp getTs() {
            return ts;
        }

        public void setTs(Timestamp ts) {
            this.ts = ts;
        }

        public double getCurrent() {
            return current;
        }

        public void setCurrent(double current) {
            this.current = current;
        }

        public int getVoltage() {
            return voltage;
        }

        public void setVoltage(int voltage) {
            this.voltage = voltage;
        }

        public double getPhase() {
            return phase;
        }

        public void setPhase(double phase) {
            this.phase = phase;
        }

        public int getGroupid() {
            return groupid;
        }

        public void setGroupid(int groupid) {
            this.groupid = groupid;
        }

        public String getLocation() {
            return location;
        }

        public void setLocation(String location) {
            this.location = location;
        }
    }
}
// ANCHOR_END: consumer_demo
