'''
Author: Hui Tong <937013596@qq.com>
Date: 2023-01-06 16:02:29
LastEditTime: 2023-01-07 22:59:00
Description: 
'''
import os
import boto3
import unittest


# 连接s3 server
access_key = "ZDTLDIA60ARFWR2W7FCP"
secret_key = "0PH7lAg6KQxMgBtJ6I8BK77VrK81LWkDbNarWMoR"
url = "http://172.17.73.114"
s3cli = boto3.session.Session(
    access_key, secret_key).client('s3', endpoint_url=url)


# 创建bucket
def create_bucket(bucket_name, acl=""):
    s3cli.create_bucket(Bucket=bucket_name, ACL=acl)


# 列出buckets
def list_buckets():
    buckets = s3cli.list_buckets()
    print(buckets)


def delete_bucket(bucket_name):
    resp = s3cli.delete_bucket(Bucket=bucket_name)
    print(resp)

# 上传对象


def put_object(file_path):
    with open(file_path, 'rb') as f:
        resp = s3cli.put_object(Bucket="client-test-bucket-1",
                                Key=file_path.split("/")[-1], Body=f.read())
    print(resp)


# 列出对象
def list_objects(bucket_name):
    resp = s3cli.list_objects(Bucket=bucket_name)
    for obj in resp['Contents']:
        print(" Key:", obj["Key"], "\n", "Size:",
              obj["Size"], "\n", "ETag:", obj["ETag"])


# 下载对象
def get_object(bucket_name, file_name):
    resp = s3cli.get_object(Bucket=bucket_name, Key=file_name.split("/")[-1])
    with open("./download_test1.txt", "wb") as f:
        f.write(resp["Body"].read())


# 删除对象
def delete_object(bucket_name, file_name):
    resp = s3cli.delete_object(
        Bucket=bucket_name, Key=file_name.split("/")[-1])
    print(resp)


# TODO: 大文件分片上传

# TODO: 断点上传

# unittest
class TestS3Op(unittest.TestCase):
    def setUp(self):
        self.bucket_name = "client-test-bucket-1"
        self.file = os.path.join(os.path.dirname(
            __file__), "files", "test1.txt")

    def test_create_bucket(self):
        create_bucket("client-test-bucket-1", 'public-read')
        create_bucket("client-test-bucket-2")

    def test_list_buckets(self):
        list_buckets()

    def test_put_object(self):
        put_object(self.file)

    def test_list_objects(self):
        list_objects(self.bucket_name)

    def test_get_object(self):
        get_object(self.bucket_name, self.file)

    def test_delete_object(self):
        delete_object(self.bucket_name, self.file)

    def test_delete_bucket(self):
        delete_bucket(self.bucket_name)


if __name__ == "__main__":
    suite = unittest.TestSuite()
    # suite.addTest(TestS3Op('test_create_bucket'))
    # suite.addTest(TestS3Op('test_list_buckets'))
    # suite.addTest(TestS3Op('test_put_object'))
    # suite.addTest(TestS3Op('test_list_objects'))
    # suite.addTest(TestS3Op('test_get_object'))
    # suite.addTest(TestS3Op('test_delete_object'))
    # suite.addTest(TestS3Op('test_delete_bucket'))

    runner = unittest.TextTestRunner()
    runner.run(suite)
